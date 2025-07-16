#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>
using namespace std;

class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int quantity;
    double margin;

public:
    Product(int id, string name, string category, double price, int quantity, double margin)
        : id(id), name(name), category(category), price(price), quantity(quantity), margin(margin) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    double getMargin() const { return margin; }
    
    void setName(string name) { this->name = name; }
    void setCategory(string category) { this->category = category; }
    void setPrice(double price) { this->price = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setMargin(double margin) { this->margin = margin; }
};

class Inventory {
private:
    vector<Product> products;
    double totalRevenue = 0;
    double totalProfit = 0;

public:
    void addProduct(Product product) {
        for (auto& p : products) {
            if (p.getId() == product.getId()) {
                cout << "Id already exists." << endl;
                return;
            }
        }
        products.push_back(product);
        double revenue = product.getPrice() * product.getQuantity();
        totalRevenue += revenue;
        totalProfit += revenue * (product.getMargin() / 100);
        cout << "Product added successfully." << endl;
    }

    void removeProduct(int id) {
        auto it = remove_if(products.begin(), products.end(),
                            [id, this](Product& p) {
                                if (p.getId() == id) {
                                    double revenue = p.getPrice() * p.getQuantity();
                                    totalRevenue -= revenue;
                                    totalProfit -= revenue * (p.getMargin() / 100);
                                    return true;
                                }
                                return false;
                            });

        if (it != products.end()) {
            products.erase(it, products.end());
            cout << "Product removed successfully." << endl;
        } else {
            cout << "Id does not exist." << endl;
        }
    }

    Product* findProduct(int id) {
        for (auto& product : products) {
            if (product.getId() == id) {
                return &product;
            }
        }
        return nullptr;
    }

    void updateProduct(int id, string name, string category, double price, int quantity, double margin) {
        Product* product = findProduct(id);
        if (product) {
            // Adjust the revenue and profit before updating
            double oldRevenue = product->getPrice() * product->getQuantity();
            totalRevenue -= oldRevenue;
            totalProfit -= oldRevenue * (product->getMargin() / 100);

            product->setName(name);
            product->setCategory(category);
            product->setPrice(price);
            product->setQuantity(quantity);
            product->setMargin(margin);

            // Recalculate revenue and profit with updated values
            double newRevenue = price * quantity;
            totalRevenue += newRevenue;
            totalProfit += newRevenue * (margin / 100);

            cout << "Product updated successfully." << endl;
        } else {
            cout << "ID does not exist." << endl;
        }
    }

    void printProducts() const {
        if (products.empty()) {
            cout << "No products in inventory." << endl;
        } else {
            for (const auto& product : products) {
                cout << "-------------------------------------------" << endl;
                cout << "ID: " << product.getId() << endl;
                cout << "Name: " << product.getName() << endl;
                cout << "Category: " << product.getCategory() << endl;
                cout << "Price: Rs." << product.getPrice() << endl;
                cout << "Quantity: " << product.getQuantity() << endl;
                cout << "Margin: " << product.getMargin() << "%" << endl;
                cout << "-------------------------------------------" << endl;
            }
        }
        cout << "Total Inventory: Rs." << totalRevenue << endl;
        cout << "Estimated Profit: Rs." << totalProfit << endl;
    }

    void saveInventoryToFile(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file for saving." << endl;
            return;
        }

        for (const auto& product : products) {
            file << product.getId() << ","
                 << product.getName() << ","
                 << product.getCategory() << ","
                 << product.getPrice() << ","
                 << product.getQuantity() << ","
                 << product.getMargin() << endl;
        }
        file.close();
        cout << "Inventory saved to file." << endl;
    }

    void loadInventoryFromFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return;
        }

        products.clear();
        totalRevenue = 0;
        totalProfit = 0;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idStr, name, category, priceStr, quantityStr, marginStr;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, quantityStr, ',');
            getline(ss, marginStr, ',');

            try {
                int id = stoi(idStr);
                double price = stod(priceStr);
                int quantity = stoi(quantityStr);
                double margin = stod(marginStr);

                products.emplace_back(id, name, category, price, quantity, margin);
                totalRevenue += price * quantity;
                totalProfit += price * quantity * (margin / 100);
            } catch (const invalid_argument& e) {
                cout << "Invalid data in file, skipping line." << endl;
            }
        }
        file.close();
        cout << "Inventory loaded from file." << endl;
    }
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    Inventory inventory;
    char choice;

    cout << "-------------------------------------------" << endl;
    cout << "khatta-Inventory Management System -" << endl;
    cout << "-------------------------------------------" << endl;

    do {
        cout << "Please choose an option:" << endl;
        cout << "1. Add a product" << endl;
        cout << "2. Remove a product" << endl;
        cout << "3. Find a product" << endl;
        cout << "4. Update a product" << endl;
        cout << "5. View all products" << endl;
        cout << "6. Save inventory to file" << endl;
        cout << "7. Load inventory from file" << endl;
        cout << "8. Display total revenue and profit" << endl;
        cout << "Q. Quit" << endl;
        cin >> choice;
        clearInput();

        switch (choice) {
            case '1': {
                int id, quantity;
                string name, category;
                double price, margin;

                cout << "Enter ID: ";
                while (!(cin >> id)) { clearInput(); }

                cout << "Enter product name: ";
                cin >> ws; getline(cin, name);

                cout << "Enter product category: ";
                cin >> ws; getline(cin, category);

                cout << "Enter product price: ₹ ";
                while (!(cin >> price)) { clearInput(); }

                cout << "Enter product quantity: ";
                while (!(cin >> quantity)) { clearInput(); }

                cout << "Enter profit margin (%): ";
                while (!(cin >> margin)) { clearInput(); }

                Product product(id, name, category, price, quantity, margin);
                inventory.addProduct(product);
                break;
            }

            case '2': {
                int id;
                cout << "Enter product id: ";
                while (!(cin >> id)) { clearInput(); }
                inventory.removeProduct(id);
                break;
            }

            case '3': {
                int id;
                cout << "Enter product id: ";
                while (!(cin >> id)) { clearInput(); }
                Product* product = inventory.findProduct(id);
                if (product) {
                    cout << "Name: " << product->getName() << endl;
                    cout << "Category: " << product->getCategory() << endl;
                    cout << "Price: Rs. " << product->getPrice() << endl;
                    cout << "Quantity: " << product->getQuantity() << endl;
                    cout << "Margin: " << product->getMargin() << "%" << endl;
                } else {
                    cout << "Product not found." << endl;
                }
                break;
            }

            case '4': {
                int id, quantity;
                string name, category;
                double price, margin;

                cout << "Enter the product id: ";
                while (!(cin >> id)) { clearInput(); }

                cout << "Enter new product name: ";
                cin >> ws; getline(cin, name);

                cout << "Enter new product category: ";
                cin >> ws; getline(cin, category);

                cout << "Enter new product price: Rs.  ";
                while (!(cin >> price)) { clearInput(); }

                cout << "Enter new product quantity: ";
                while (!(cin >> quantity)) { clearInput(); }

                cout << "Enter new profit margin (%): ";
                while (!(cin >> margin)) { clearInput(); }

                inventory.updateProduct(id, name, category, price, quantity, margin);
                break;
            }

            case '5':
                inventory.printProducts();
                break;

            case '6': {
                string filename;
                cout << "Enter filename to save inventory: ";
                cin >> filename;
                inventory.saveInventoryToFile(filename);
                break;
            }

            case '7': {
                string filename;
                cout << "Enter filename to load inventory: ";
                cin >> filename;
                inventory.loadInventoryFromFile(filename);
                break;
            }

            case '8':
                inventory.printProducts();
                break;

            case 'q':
            case 'Q':
                cout << "Goodbye!" << endl;
                return 0;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (true);

    return 0;
}
