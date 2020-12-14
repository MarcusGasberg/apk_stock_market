#include "headers/StockMediator.h"
#include "headers/StockProvider.h"
#include "headers/TraderAccount.h"

#pragma region

int main()
{
    StockProvider * stockProvider = new StockProvider;
    StockMediator<void, int, int> mediator;
    TraderAccount traderAccount(std::move(mediator));

    traderAccount.mediator_.subscribe("buy", &StockProvider::stockHasBeenBought, stockProvider);

    traderAccount.buy();

    traderAccount.mediator_.unSubscribe("buy", stockProvider);

    traderAccount.buy();

    return 0;
}

#pragma endregion