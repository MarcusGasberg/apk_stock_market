#include "headers/StockMediator.h"
#include "headers/StockProvider.h"
#include "headers/TraderAccount.h"
#include "headers/TraderTopics.h"

#pragma region

/*
int main()
{
    StockProvider * stockProvider = new StockProvider;
    StockMediator<void, int, int> mediator;
    TraderAccount traderAccount(std::move(mediator));

    boost::signals2::connection connection = traderAccount.mediator_.subscribe(TOPICS[TraderTopics::BUY], &StockProvider::stockHasBeenBought, stockProvider);

    traderAccount.buy();

    traderAccount.mediator_.unSubscribe(TOPICS[TraderTopics::BUY], connection);

    traderAccount.buy();

    return 0;
} */


#include "headers/TimeLiteral.h"
int main()
{
    "Hello World"_print;
}

#pragma endregion