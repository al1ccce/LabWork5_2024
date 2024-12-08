#include "structure.h"


void addCustomer(MarketPlace& market) {
    int id, balance;
    cout << "id: "; cin >> id;
    cout << "баланс: "; cin >> balance;
    market.add_customer(Customer(id, balance));
    customerToFile(Customer(id, balance));
}

void addSeller(MarketPlace& market) {
    string name; 
    int id;
    cout << "имя: "; cin >> name;
    cout << "id: "; cin >> id;
    market.add_seller(Seller(name, id));
    sellerToFile(Seller(name, id));
    
}

void addProduct(MarketPlace& market) {
    string name;
    int price;
    int amount;
    int seller_id;
    int id;
    cout << "имя: "; cin >> name;
    cout << "цена: "; cin >> price;
    cout << "количество: "; cin >> amount;
    cout << "id продавца: "; cin >> seller_id;
    cout << "id товара: "; cin >> id;
    productToFile(Product(name, price, amount, seller_id, id));
}

void buyProduct(MarketPlace& market) {
    int cid;
    int id;
    int amount;
    int paywhat;
    cout << "id покупателя: "; cin >> cid;
    cout << "id товара: "; cin >> id;
    cout << "количество: "; cin >> amount;
    cout << "оплата 1 - наличными, 2 - картой, 3 - криптовалютой"; cin >> paywhat;
    Payment* payment = nullptr;
    switch (paywhat) {
        case 1:
            payment = new CashPayment();
            break;
        case 2:
            payment = new CardPayment();
            break;
        case 3:
            payment = new CryptoPayment();
            break;
        default:
            cout << "Неверный выбор метода оплаты." << endl;
            return;
    }

    auto customer_it = market.customers.find(cid);
    if (customer_it == market.customers.end()) {
        cout << "Покупатель с таким id не найден." << endl;
        delete payment;
        return;
    }

    Customer& customer = customer_it->second;
    customer.buy(market, id, amount, payment);
}

void printInfo(MarketPlace& market) {
    market.print_info();
}

int main(){
    MarketPlace market;
    marketplaceFromFile(market);
    cout << "1. Добавить покупателя\n2. Добавить продавца\n3. Купить товар\n4. Добавить товар\n5. Информация о маркетплейсе\n";
    while(true){
        int v;
        try {
            cin >> v;
        } catch (...) {
            cout << "Введите число!" << endl;
            break;
        }
        switch (v) {
            case 1:
                addCustomer(market);
                break;
            case 2:
                addSeller(market);
                break;
            case 3:
                buyProduct(market);
                break;
            case 4:
                addProduct(market);
                break;
            case 5:
                printInfo(market);
                break;
            default:
                cout << "Нет такого варианта. Повторите ввод" << endl;
                break;
        }
    }

    return 0;
}

