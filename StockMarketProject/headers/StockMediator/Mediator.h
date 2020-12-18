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
 *  TraderAccount will signal that an topic has been activated (BUY / SELL)
 *  Mediator will find associated providers and update them via callback with whatever parameters is provided by TraderAccount
 *  Provider will use the parameters to do some action.
 *  This class is generic and could be used for other purposes!
 */
// At the moment mediator only supports one type of signal.
    template<typename returnType, typename... params>
    class Mediator {
    public:
        // Reference to StockProvider callback, activated upon notification.
        typedef boost::signals2::signal<returnType(params ...)> ProviderSignal;

        // String id to identify subscriber topic and all StockProvider callbacks
        typedef std::map<std::string, ProviderSignal> TopicMap;

        typedef std::pair<typename TopicMap::iterator, bool> TopicInsertResult;

        // Actual map that contains callbacks.
        TopicMap topicMap;

        // TODO: Change T with actual provider or provider interface
        // TODO: Shared_pointer / Unique_pointer for connection?
        template<typename T>
        boost::signals2::connection
        subscribe(std::string && topic, returnType (T::* providerCallback)(params ...), T *providerObject) {
            // 1. Check if topic exists
            // 2. Check if provider already registered
            // 3. Add callback to topic map.

            typename TopicMap::iterator topicIterator = std::find_if(topicMap.begin(), topicMap.end(),
                                                                     [&](const auto &map) {
                                                                         return map.first == topic;
                                                                     });

            // TODO: Use boost::ref or boost::cref?
            // boost::bind(providerCallback, providerObject, 3);
            //auto bound = custom_bind(providerCallback, providerObject);
            //auto callback = boost::function<returnType (params ...)>(bound);
            auto callback = custom_bind(providerCallback, providerObject);

            // Topic does not exist.
            if (topicIterator == topicMap.end()) {
                ProviderSignal signal;
                std::cout << "Connecting: " << typeid(providerObject).name() << " to NEW topic: " << topic << std::endl;
                auto connection = signal.connect(callback);

                TopicInsertResult ir = topicMap.insert(std::make_pair(std::move(topic), std::forward<ProviderSignal>(signal)));
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

        // TODO: Make connection be handled by mediator.
        // TODO: Make connections tracked.
        // Ownership of topic and connection is moved to mediator.
        void unSubscribe(std::string && topic, boost::signals2::connection && connection) {
            // Only works if provider has equality operator.
            if (connection.connected()) {
                // 1. Check if topic exists
                // 2. Remove connection for that topic.
                auto topicIterator = std::find_if(topicMap.begin(), topicMap.end(), [&](const auto &map) {
                    return map.first == topic;
                });

                // Topic does exist.
                if (topicIterator != topicMap.end()) {
                    std::cout << "Disconnecting from topic: " << topicIterator->first << std::endl;
                    connection.disconnect();
                }
            }
        }

        void notify(std::string && topic, params &&... parameters) {
            typename TopicMap::iterator topicIterator = std::find_if(topicMap.begin(), topicMap.end(),
                                                                     [&](const auto &map) {
                                                                         return map.first == topic;
                                                                     });

            if (topicIterator != topicMap.end()) {
                topicIterator->second(parameters ...);
            }
        }
    };
}
#endif //BOOST_SIGNAL_EXAMPLE_STOCKMEDIATOR_H
