//
// Created by stud on 12/14/20.
//

#ifndef BOOST_SIGNAL_EXAMPLE_STOCKPROVIDER_H
#define BOOST_SIGNAL_EXAMPLE_STOCKPROVIDER_H

#include <iostream>

class StockProvider {
private:
    std::string name = "Novo Nordisk";
public:
    // Example callback
    void stockHasBeenBought(int amount, int price)
    {
        // Do something.
        std::cout << "Someone bought " << amount << " stocks for the price of " << price << " with id " << "314d4123" << std::endl;
    }

};

#endif //BOOST_SIGNAL_EXAMPLE_STOCKPROVIDER_H
