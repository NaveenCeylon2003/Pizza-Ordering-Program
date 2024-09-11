#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

struct PizzaOrder
{
    string size;
    vector<string> toppings;
};

void handleInvalidInput()
{
    cout << "Invalid Input. Please enter a valid number:\n";
    cin.clear();
    cin.ignore(1000, '\n');
}

void addcredits(double &credits, double &totalcost)
{
    cout << "\nEnter your credit balance to order your pizza: ";
    double input;
    cin >> input;
    while (cin.fail() || input < 0)
    {
        handleInvalidInput();
        cin >> input;
    }
    credits += input;
    cout << "\nAvailable Balance: " << fixed << setprecision(2) << credits - totalcost << " credits\n";
}

void entrance(double &credits, double &totalcost) // displays the main menu
{
    cout << "----------------------------------------\nUCLan PIZZA\n----------------------------------------\nMAIN MENU\n\t1. Add Credits (Current credits = " << fixed << setprecision(2) << credits - totalcost << ")\n\t2. Order Pizza\n\t3. View Recent Orders\n\t0. Exit\nPlease enter a number: ";
}

int ordersize()
{
    int sizeorder;
    cout << "\n====Size Chart====\n1. Small\t[5.0 credits]\n2. Medium\t[8.50 credits]\n3. Large\t[10.25 credits]\n0. Return to Main Menu or Finish order\n........................................\nPlease Enter a number: ";
    cin >> sizeorder;
    while (cin.fail())
    {
        handleInvalidInput();
        cin >> sizeorder;
    }
    return sizeorder;
}

int toppingloop(vector<string> &toppingsordered, double &cost) // to add toppings to the order
{
    int topping = 1; // initialized to meet the loop condition
    const string toppingname[6] = {"Ham", "Mushrooms", "Pepperoni", "Peppers", "Onions", "Extra Cheese"};
    double toppingcost[6] = {0.8, 0.5, 1, 0.8, 0.4, 1.5};

    cout << "\nPlease choose from the following toppings:\n\n1. Ham\t\t\t[0.80 credits]\n2. Mushrooms\t\t[0.50 credits]\n3. Pepperoni\t\t[1.00 credits]\n4. Peppers\t\t[0.80 credits]\n5. Onions\t\t[0.40 credits]\n6. Extra Cheese\t\t[1.50 credits]\n0. Finish Order\n........................................\nPlease enter a number: ";
    cin >> topping;
    while (cin.fail())
    {
        handleInvalidInput();
        cin >> topping;
    }

    while (topping != 0)
    {
        if (topping >= 1 && topping <= 6)
        {
            cost += toppingcost[topping - 1];
            toppingsordered.push_back(toppingname[topping - 1]);
            cout << "You ordered " << toppingname[topping - 1] << " topping." << "\nYour cost is " << fixed << setprecision(2) << cost << " credits" << "\n\nEnter a number to select another topping or 0 to finish order: ";
        }
        else
        {
            cout << "Invalid Input. Select an integer between 0 and 6\n:";
        }
        cin >> topping;
        while (cin.fail())
        {
            handleInvalidInput();
            cin >> topping;
        }
    }

    return topping;
}

void orderprocess(vector<PizzaOrder> &orders, double &credits, double &cost, double &totalcost)
{
    int sizeorder = ordersize();
    if (sizeorder == 0)
        return;

    const string sizechart[3] = {"Small", "Medium", "Large"};
    double sizecost[3] = {5.0, 8.5, 10.25};

    PizzaOrder newOrder; // Create a new PizzaOrder struct for this order

    if (sizeorder >= 1 && sizeorder <= 3)
    {
        cost = sizecost[sizeorder - 1];
        cout << "You have ordered a " << sizechart[sizeorder - 1] << " Pizza\n";
        newOrder.size = sizechart[sizeorder - 1];
    }
    else
    {
        cout << "Invalid Input, Select from menu\n";
        orderprocess(orders, credits, cost, totalcost);
        return; // Return after handling invalid input
    }

    int topping = toppingloop(newOrder.toppings, cost);
    if (newOrder.toppings.empty())
    {
        cost = 0;
        cout << "At least one topping is mandatory to confirm your order. Try again\n";
        return;
    }

    totalcost += cost;

    if (credits < totalcost)
    {
        char morecredits;
        cout << "You have insufficient credits. You require " << fixed << setprecision(2) << totalcost - credits << " credits. Would you like to add more credits? (Y/N): ";
        cin >> morecredits;

        if (morecredits == 'Y' || morecredits == 'y')
        {
            addcredits(credits, totalcost);
            return;
        }
        else
        {
            cout << "Thank you, Goodbye!\n";
            return;
        }
    }
    else
    {
        orders.push_back(newOrder); // Add the new order to the orders vector
        cout << "Your order has been placed!\n";
    }
}

void recent(vector<PizzaOrder> &orders, double &credits, double &totalcost)
{
    cout << "Recent Orders:\n........................................\n";
    if (orders.empty())
    {
        cout << "No recent orders.\n";
        return;
    }

    ofstream invoice{"Invoice.txt"};
    if (!invoice)
    {
        cout << "File not opened, Try again\n";
        return;
    }
    invoice << "........................................\n\tUCLAN PIZZA-INVOICE\n........................................\n";
    for (int i = 0; i < orders.size(); i++)
    {
        cout << orders[i].size << " Pizza with ";
        invoice << orders[i].size << " Pizza with ";

        for (int j = 0; j < orders[i].toppings.size(); j++)
        {
            cout << orders[i].toppings[j];
            invoice << orders[i].toppings[j];

            if (j < orders[i].toppings.size() - 1)
            {
                cout << ", ";
                invoice << ", ";
            }
        }

        cout << endl;
        invoice << endl;
    }

    cout << "Credits received: " << fixed << setprecision(2) << credits << "\nYour total cost: " << fixed << setprecision(2) << totalcost << "\nAvailable credit balance: " << fixed << setprecision(2) << credits - totalcost << "\nCheck files for invoice\n";
    invoice << "\n........................................\nCredits received: " << fixed << setprecision(2) << credits << "\nYour total cost: " << fixed << setprecision(2) << totalcost << "\nAvailable credit balance: " << fixed << setprecision(2) << credits - totalcost << "\n........................................\nThank you! Goodbye.";
    invoice.close();
}

int main()
{
    int menu;
    double credits = 0.0;      // Initialize credits to 0.0
    double cost = 0.0;         // Initialize cost to 0.0
    double totalcost = 0.0;    // Initialize totalcost to 0.0
    vector<PizzaOrder> orders; // Vector to store all pizza orders

    do
    {
        entrance(credits, totalcost);
        cin >> menu;
        while (cin.fail())
        {
            handleInvalidInput();
            cin >> menu;
        }

        switch (menu)
        {
        case 1:
            addcredits(credits, totalcost);
            break;

        case 2:
            orderprocess(orders, credits, cost, totalcost);
            break;

        case 3:
            recent(orders, credits, totalcost);
            break;

        case 0:
            cout << "Thank you, Goodbye!\n";
            break;

        default:
            cout << "Please Enter a valid number\n";
            break;
        }

    } while (menu != 0);
    return 0;
}
