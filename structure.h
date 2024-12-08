#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

class Payment {
public:
    virtual ~Payment() = default;
    virtual void pay(int dengi) = 0;
};

class CashPayment : public Payment {
public:
    void pay(int dengi) override {
        cout << "Оплачено " << dengi << " наличными" << endl;
    }
};

class CardPayment : public Payment {
public:
    void pay(int dengi) override {
        cout << "Оплачено " << dengi << " дебетовой картой" << endl;
    }
};

class CryptoPayment : public Payment {
public:
    void pay(int dengi) override {
        cout << "Оплачено " << dengi << " криптовалютой" << endl;
    }
};

class Product {
public:
    string name;
    int price;
    int amount;
    int seller_id;
    int id;

    Product() : name(""), price(0), amount(0), seller_id(0), id(0) {}
    Product(string _name, int _price, int _amount, int sid, int pid)
        : name(_name), price(_price), amount(_amount), seller_id(sid), id(pid) {}
};

class MarketPlace;

class Customer {
public:
    int id;

    Customer() : id(0), balance(0) {}
    Customer(int id, int balance) : id(id), balance(balance) {}

    int get_balance() const;
    void buy(MarketPlace& shop, int product_id, int amount, Payment* payment);

private:
    int balance;
};

class Seller {
public:
    string name;
    int id;

    Seller() : name(""), id(0) {}
    Seller(string name, int id) : name(name), id(id) {}

    void add_product(MarketPlace& shop, Product product);

};

class MarketPlace {
    friend class Customer;
    friend class Seller;

public:
    unordered_map<int, Seller> sellers;
    unordered_map<int, Customer> customers;
    unordered_map<int, Product> products;

    void add_customer(Customer customer) {
        if (customers.find(customer.id) != customers.end()) {
            cout << "Такой идентификатор (id" << customer.id << ") уже есть, придумайте другой" << endl;
        } else {
            customers[customer.id] = customer;
        }
    }

    void add_seller(Seller seller) {
        if (sellers.find(seller.id) != sellers.end()) {
            cout << "Такой идентификатор (id" << seller.id << ") уже есть, придумайте другой" << endl;
        } else {
            sellers[seller.id] = seller;
        }
    }

    void add_product(Product product) {
        if (products.find(product.id) != products.end()) {
            cout << "Такой идентификатор уже есть, придумайте другой" << endl;
        } else {
            if (sellers.find(product.seller_id) == sellers.end()) {
                cout << "Продавца не существует на площадке" << endl;
            } else {
                products[product.id] = product;
                cout << "Товар " << product.name << ", (id" << product.id << ") добавлен на площадку" << endl;
            }
        }
    }

    void print_info() const {
        cout << "Список продавцов на маркетплейсе: " << endl;
        for (const auto& seller : sellers) {
            cout << "Имя: " << seller.second.name << ", ID: " << seller.second.id << endl;
        }

        cout << "\nСписок покупателей на маркетплейсе: " << endl;
        for (const auto& customer : customers) {
            cout << "ID: " << customer.second.id << ", Баланс: " << customer.second.get_balance() << endl;
        }

        cout << "\nСписок товаров на маркетплейсе: " << endl;
        for (const auto& product : products) {
            cout << "Название: " << product.second.name << ", Цена: " << product.second.price
                 << ", Количество: " << product.second.amount << ", ID продавца: " << product.second.seller_id
                 << ", ID товара: " << product.second.id << endl;
        }
        cout << endl;
    }

private:
    void reduce_product(int id, int amount) {
        auto it = products.find(id);
        if (it != products.end()) {
            it->second.amount -= amount;
        }

        fstream file("products.data", ios::in | ios::out);
        string line;
        stringstream newline;

        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            int price, amnt, seller_id, product_id;
            ss >> name >> price >> amnt >> seller_id >> product_id;

            if (product_id == id) {
                amnt -= amount;
            }

            newline << name << " " << price << " " << amnt << " " << seller_id << " " << product_id << endl;
        }

        file.close();
        file.open("products.data", ios::out | ios::trunc);
        file << newline.str();
        file.close();
    }
};


void marketplaceFromFile(MarketPlace& market);
void customerToFile(Customer customer);
void sellerToFile(Seller seller);
void productToFile(Product product);