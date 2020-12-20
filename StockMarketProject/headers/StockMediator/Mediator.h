//
// Created by stud on 12/14/20.
//

#ifndef BOOST_SIGNAL_EXAMPLE_STOCKMEDIATOR_H
#define BOOST_SIGNAL_EXAMPLE_STOCKMEDIATOR_H
#include "Placeholders.h"
#include "boost/signals2.hpp"
#include "iostream"

namespace stock {
/*
 *  TraderAccount will signal that a topic has been activated (BUY / SELL)
 *  Mediator will find associated providers and update them via callback with whatever parameters is provided by TraderAccount
 *  Provider will use the parameters to do some action.
 */
    template<typename returnType, typename... params>
    class Mediator {
        // Reference to StockBroker callback, activated upon notification.
        typedef boost::signals2::signal<returnType(params ...)> provider_signal;

        // String id to identify subscriber topic and all StockBroker callbacks
        typedef std::map<std::string, provider_signal> topic_map;

        typedef std::pair<typename topic_map::iterator, bool> topic_insert_result;
        // Actual map that contains callbacks.
        topic_map topic_map_;
    public:
        // TODO: Shared_pointer / Unique_pointer for connection?
        template<typename T>
        boost::signals2::connection
        subscribe(std::string && topic, returnType (T::* providerCallback)(params ...), T *providerObject) {
            typename topic_map::iterator topicIterator = std::find_if(topic_map_.begin(), topic_map_.end(),
                                                                      [&](const auto &map) {
                                                                         return map.first == topic;
                                                                     });

            auto callback = custom_bind(providerCallback, providerObject);

            if (topicIterator == topic_map_.end()) {
                provider_signal signal;
                std::cout << "Connecting: " << typeid(providerObject).name() << " to NEW topic: " << topic << std::endl;
                auto connection = signal.connect(callback);

                // TODO: Check this forwarding.
                topic_insert_result ir = topic_map_.insert(std::make_pair(std::move(topic), std::forward<provider_signal>(signal)));
                if (!ir.second) {
                    throw "An error occurred when inserting topic map.";
                }
                return connection;
            } else {
                std::cout << "Connecting: " << typeid(providerObject).name() << " to EXISTING topic: " << topic
                          << std::endl;
                return topicIterator->second.connect(callback);
            }
        }

        // TODO: Move is not okay, if disconnect is NOT called, since ownership has changed.
        void unSubscribe(std::string && topic, boost::signals2::connection && connection) {
            if (connection.connected()) {

                auto topicIterator = std::find_if(topic_map_.begin(), topic_map_.end(), [&](const auto &map) {
                    return map.first == topic;
                });

                if (topicIterator != topic_map_.end()) {
                    std::cout << "Disconnecting from topic: " << topicIterator->first << std::endl;
                    connection.disconnect();
                }
            }
        }

        void notify(std::string && topic, params &&... parameters) {
            typename topic_map::iterator topicIterator = std::find_if(topic_map_.begin(), topic_map_.end(),
                                                                      [&](const auto &map) {
                                                                         return map.first == topic;
                                                                     });

            if (topicIterator != topic_map_.end()) {
                topicIterator->second(parameters ...);
            }
        }
    };
}
#endif //BOOST_SIGNAL_EXAMPLE_STOCKMEDIATOR_H
