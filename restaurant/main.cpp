#include<iostream>
#include<cstdio>
#include<string>
#include<windows.h>
#include<ctime>
using namespace std;

struct MenuItem {
    int productNumber;
    string productName;
    double price;
    MenuItem* left;
    MenuItem* right;

    MenuItem(int number, string name, double cost) : productNumber(number), productName(name), price(cost), left(nullptr), right(nullptr) {}
};

// Function to create a new MenuItem
MenuItem* createMenuItem(int number, string name, double price) {
    return new MenuItem(number, name, price);
}

// Function to insert a MenuItem into the binary search tree
void insertMenuItem(MenuItem*& root, MenuItem* newItem) {
    if (!root) {
        root = newItem;
    } else {
        if (newItem->productNumber < root->productNumber) {
            insertMenuItem(root->left, newItem);
        } else {
            insertMenuItem(root->right, newItem);
        }
    }
}

// Function to perform post-order traversal and display menu items
void displayMenuPostOrder(MenuItem* root) {
    if (root) {
        displayMenuPostOrder(root->left);
        displayMenuPostOrder(root->right);
        cout << "\t\t\t" << root->productNumber << "\t\t\t" << root->productName << "\t\t\tPhp " << root->price << endl;
    }
}

struct Order {
    MenuItem* item;
    int quantity;
    Order* next;

    Order(MenuItem* menuItem, int qty) : item(menuItem), quantity(qty), next(nullptr) {}
};

void cls() {
    system("cls");
}

void addOrder(Order*& head, MenuItem* item, int quantity) {
    Order* newOrder = new Order(item, quantity);
    if (!head) {
        head = newOrder;
    } else {
        Order* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newOrder;
    }
}

void deleteOrder(Order*& head, int orderNum) {
    if (!head) {
        cout << "No orders to delete." << endl;
        return;
    }

    Order* temp = head;
    Order* prev = nullptr;

    while (temp && temp->item->productNumber != orderNum) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Order not found." << endl;
        return;
    }

    if (!prev) {
        head = head->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;
    cout << "Order deleted successfully." << endl;
}

void displayOrders(Order* head) {
    if (head == nullptr) {
        cout << "\n\t\tNo orders placed yet.\n";
        return;
    }

    cout << "\n\t\tAll Orders:\n";
    cout << "\t\tProduct Name\t\tQuantity\tPrice\t\tTotal\n";

    Order* currentOrder = head;
    while (currentOrder) {
        double amount = currentOrder->item->price * currentOrder->quantity;
        cout << "\t\t" << currentOrder->item->productName << "\t\t    " << currentOrder->quantity << "\t\tPhp " << currentOrder->item->price << "\tPhp " << amount << endl;
        currentOrder = currentOrder->next;
    }
}

int main() {
    system("mode 100");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    string username, password;
    int order, additional;
    int input = 0;
    double total = 0;
    MenuItem* menu = nullptr;
    Order* orders = nullptr;

    insertMenuItem(menu, createMenuItem(1, "Hamburger", 25.00));
    insertMenuItem(menu, createMenuItem(2, "Egg Sandwich", 25.00));
    // Add other menu items...

    do {
        // Login...
        cout << "\n\n\t\t\t=====>>>>WELCOME TO EL WENSH<<<<=====\n\n";
        cout << "\t\t\t      Please Login to Continue" << endl;
        cout << "\t\t\t      User Name: ";
        cin >> username;

        if (username == "rawan" || username == "drtamer" || username == "menna") {
            cout << "\t\t\t      Password: ";
            cin >> password;
            cout << endl;
            if (password != "user") {
                cls();
                cout << "\n\t\t\tInvalid password. Try again." << endl;
            }
        } else {
            cls();
            cout << "\n\t\t\tInvalid username. Try again." << endl;
        }

        if (password == "user") {
            do {
                // Display Menu...
                cout << "\n\n\t\t\t=====>>>>PICK YOUR ORDER<<<<=====\n\n";
                cout << "\t\t=====================================================================\n";
                cout << "\t\t\t  PRODUCT NUMBER\tPRODUCT\t\t\tPRICE\n";
                cout << "\t\t=====================================================================\n";
                displayMenuPostOrder(menu);
                cout << "\t\t=====================================================================\n";

                do {
                    // Select and add order...
                    cout << "\n\n\t\tSelect your order: ";
                    cin >> order;

                    // Search for the selected menu item
                    MenuItem* selectedMenuItem = menu;
                    while (selectedMenuItem) {
                        if (selectedMenuItem->productNumber == order) {
                            int quantity;
                            cout << "\t\tHow many " << selectedMenuItem->productName << " would you like to order: ";
                            cin >> quantity;
                            addOrder(orders, selectedMenuItem, quantity);
                            break;
                        }
                        selectedMenuItem = (order < selectedMenuItem->productNumber) ? selectedMenuItem->left : selectedMenuItem->right;
                    }

                    cout << "\t\tDo you want to add more? [ 0 ] no [ 1 ] yes: ";
                    cin >> additional;
                    if (additional == 0) {
                        // Calculate total and handle payment...
                        cout << fixed;
                        cout.precision(2);

                        cout << "\n\n\n\t\tYou have ordered\n";
                        cout << "\t\tProduct Name\t\tQuantity\tPrice\t\tTotal\n";

                        // Display Ordered Items...
                        Order* currentOrder = orders;
                        while (currentOrder) {
                            double amount = currentOrder->item->price * currentOrder->quantity;
                            cout << "\t\t" << currentOrder->item->productName << "\t\t    " << currentOrder->quantity << "\t\tPhp " << currentOrder->item->price << "\tPhp " << amount << endl;
                            total += amount;
                            currentOrder = currentOrder->next;
                        }

                        cout << "\n\t\tTotal Amount Due: Php " << total << endl;
                        int payment;
                        do {
                            cout << "\n\t\tPayment: Php ";
                            cin >> payment;

                            if (payment < total) {
                                cout << "\n\t\tInsufficient amount." << endl;
                            }
                        } while (payment < total);

                        double change = payment - total;

                        // Display receipt...
                        cout << "\n\t\tProduct Name\t\tQuantity\tPrice\t\tTotal\n";
                        Order* receiptOrder = orders;
                        while (receiptOrder) {
                            double amount = receiptOrder->item->price * receiptOrder->quantity;
                            cout << "\t\t" << receiptOrder->item->productName << "\t\t    " << receiptOrder->quantity << "\t\tPhp " << receiptOrder->item->price << "\tPhp " << amount << endl;
                            receiptOrder = receiptOrder->next;
                        }

                        cout << "\n\t\tTotal Amount Due: Php " << total << endl;
                        cout << "\t\tPayment: Php " << payment << endl;
                        cout << "\t\tChange: Php " << change << endl;
                        cout << "\t\t================================================" << endl;
                        cout << "\n\t\tTHANK YOU FOR YOUR PURCHASE" << endl;
                        cout << "\t\t================================================" << endl;

                        total = 0;

                        // Ask if the user wants to make another order...
                        cout << "\n\t\tDo you want to:\n";
                        cout << "\t\t[1] Make another order\n";
                        cout << "\t\t[2] Delete an order\n";
                        cout << "\t\t[3] Show all orders\n";
                        cout << "\t\t[0] Exit\n";
                        cout << "\t\tEnter your choice: ";
                        cin >> input;

                        if (input == 1) {
                            cls();
                            break; // Exit the order loop to start a new order
                        } else if (input == 2) {
                            // Delete an order
                            int orderNum;
                            cout << "\n\t\tEnter the order number to delete: ";
                            cin >> orderNum;
                            deleteOrder(orders, orderNum);
                        } else if (input == 3) {
                            // Display all orders
                            cls(); // Clear screen
                            displayOrders(orders);
                        } else if (input == 0) {
                            // Exit the program
                            cls();
                            cout << "\n\t\tThank you for using el wensh! Goodbye!" << endl;
                            return 0;
                        } else {
                            cls();
                            cout << "\n\t\tInvalid input. Please try again." << endl;
                        }
                    }
                } while (true);
            } while (true);
        }
    } while (true);

    return 0;
}
