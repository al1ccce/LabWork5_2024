#include "structure.h"

void marketplaceFromFile(MarketPlace& market) {
    ifstream customersFile("customers.data");
    ifstream sellersFile("sellers.data");
    ifstream productsFile("products.data");

    string line;

    while (getline(customersFile, line)) {
        stringstream ss(line);
        int id, balance;
        ss >> id >> balance;
        market.add_customer(Customer(id, balance));
    }

    while (getline(sellersFile, line)) {
        stringstream ss(line);
        string name;
        int id;
        ss >> name >> id;
        market.add_seller(Seller(name, id));
    }

    while (getline(productsFile, line)) {
        stringstream ss(line);
        string name;
        int price, amount, seller_id, id;
        ss >> name >> price >> amount >> seller_id >> id;
        market.add_product(Product(name, price, amount, seller_id, id));
    }
    customersFile.close();
    sellersFile.close();
    productsFile.close();
}

void customerToFile(Customer customer){
    ofstream out("customers.data", ios::app);
    out << customer.id << " " << customer.get_balance() << endl;
    out.close();
}

void sellerToFile(Seller seller){
    ofstream out("sellers.data", ios::app);
    out << seller.name << " " << seller.id << endl;
    out.close();
}

void productToFile(Product product){
    ofstream out("products.data", ios::app);
    out << product.name << " " << product.price << " " << product.amount << " " << product.seller_id << " " << product.id << endl;
    out.close();
}

void Customer::buy(MarketPlace& shop, int product_id, int amount, Payment* payment) {
    auto it = shop.products.find(product_id);
    if (it == shop.products.end()) {
        cout << "Такого товара не существует" << endl;
        return;
    }
    if (it->second.amount < amount) {
        cout << "Недостаточно товара на складе" << endl;
        return;
    }
    int price = it->second.price;
    if (price * amount > balance) {
        cout << "Недостаточно средств на балансе кошелька" << endl;
        return;
    }
    balance -= price * amount;
    shop.reduce_product(product_id, amount);
    payment->pay(price * amount);

    cout << "\nЧек:" << endl;
    cout << "Покупатель: " << id << endl;
    cout << "Товар: " << it->second.name << endl;
    cout << "Количество: " << amount << endl;
    cout << "Цена за единицу: " << price << endl;
    cout << "Общая стоимость: " << price * amount << endl;
    cout << "Метод оплаты: ";
    payment->pay(price * amount); 
    cout << "Остаток на балансе: " << balance << endl;
    cout << "----------------------------------------" << endl;
}


int Customer::get_balance() const{
        return balance;
    }

void Seller::add_product(MarketPlace& shop, Product product) {
    if (shop.products.find(product.id) != shop.products.end()) {
        cout << "Такой товар уже существует" << endl;
    } else {
        shop.add_product(product);
    }
}