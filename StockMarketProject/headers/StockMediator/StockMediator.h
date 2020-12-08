#include <iostream>
#include "boost/signals2.hpp"
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>

template<typename returnType, typename... params>
struct StockMediator
{
    // Reference to StockProvider callback, activated upon notification.
    typedef boost::signals2::signal<returnType (params ...)> ProviderSignal;

    // String id to identify the different stock provider's callback
    typedef std::vector<std::map<std::string, ProviderSignal>> ProviderContainer;

    // String id to identify subscriber topic and all StockProvider callbacks
    typedef std::map<std::string, ProviderContainer> TopicMap;

    TopicMap topicMap;

    // TODO: Change T with actual provider or provider interface?
    template<typename T>
    void subscribe(std::string & topic, std::string & providerId, returnType (T::* providerCallback)(params ...), T * providerObject)
    {
        // Check if topic already exists.
        // TODO: Use iterators!
        auto topicIterator = std::find(topicMap.begin(), topicMap.end(), [topic](TopicMap & topicMap){
            return topicMap.first == topic;
        });

        // Topic does not exist.
        if(topicIterator == topicMap.end())
        {
            // TODO: Throw exception or return e.g. boolean?
            return;
        }

        // Get reference to ProviderContainer from TopicMap.
        ProviderContainer & providerContainer = topicIterator->second;

        // Insert new ProviderContainer with ProviderCallback.
        auto bound = boost::bind(&providerCallback, providerObject, std::forward<params>()...);
        auto callback = boost::function<returnType (params ...)>(bound);

        ProviderSignal signal;
        signal.connect(callback);
        providerContainer.insert(std::make_pair(providerId, signal));
    }

    void unSubscribe()
    {
        
    }
};