#include "server.h"
#include <iomanip>
#include <memory>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include "client.h"
#include "crypto.h"

int Rand(int begin, int end) {
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<int> u(begin, end);
    return u(e);
}

std::vector<std::string> pending_trxs;

Server::Server() {}

auto Server::add_client(std::string id) -> std::shared_ptr<Client> {
    bool is_exist = false;
    for (const auto &[client_ptr, _] : clients) {
        if (client_ptr->get_id() == id) {
            is_exist = true;
        }
    }
    if (is_exist) {
        std::stringstream ss;
        std::string rand_suffix;
        ss << std::setw(4) << std::setfill('0') << Rand(0, 9999) << " ";
        ss >> rand_suffix;
        id = id + rand_suffix;
    }
    auto client_ptr = std::make_shared<Client>(id, *this);
    clients[client_ptr] = 5;
    return client_ptr;
}

auto Server::get_client(std::string id) const -> std::shared_ptr<Client> {
    for (const auto &[client_ptr, _] : clients) {
        if (client_ptr->get_id() == id) {
            return client_ptr;
        }
    }
    return nullptr;
}

auto Server::get_wallet(std::string id) const -> double {
    for (const auto &[client_ptr, balance] : clients) {
        if (client_ptr->get_id() == id) {
            return balance;
        }
    }
    return 0;
}

auto Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value) -> bool {
    std::vector<std::string> res;
    std::regex reg("-");
    std::sregex_token_iterator pos(trx.begin(), trx.end(), reg, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) {
        res.push_back(pos->str());
    }
    if (res.size() != 3) {
        throw std::runtime_error("wrong format trx");
    }
    // todo determine if the last string is a number
    sender = res.front();
    receiver = res[1];
    value = std::stod(res.back());
    return true;
}

auto Server::add_pending_trx(std::string trx, std::string signature) const -> bool {
    std::string sender;
    std::string receiver;
    double value;
    if (!parse_trx(trx, sender, receiver, value)) {
        return false;
    }
    auto sender_ptr = get_client(sender);
    auto receiver_ptr = get_client(receiver);
    if (!sender_ptr || !receiver_ptr) {
        return false;
    }
    bool authentic = crypto::verifySignature(sender_ptr->get_publickey(), trx, signature);
    if (!authentic) {
        return false;
    }
    if (get_wallet(sender) < value) {
        return false;
    }
    pending_trxs.push_back(trx);
    return true;
}

auto Server::mine() -> size_t {
    std::string mempool;
    for (const auto &trx : pending_trxs) {
        mempool += trx;
    }
    bool success = false;
    size_t success_nonce;
    std::shared_ptr<Client> mine_success_client;
    while (!success) {
        for (const auto &[client_ptr, _] : clients) {
            auto nonce = client_ptr->generate_nonce();
            auto tmp = mempool + std::to_string(nonce);
            std::string hash{crypto::sha256(tmp)};
            for (size_t i = 0; i < 10 && i < hash.size(); i++) {
                int cnt = 0;
                while (hash[i] == '0' && i < 10 && i < hash.size()) {
                    cnt++;
                    i++;
                }
                if (cnt >= 3) {
                    success_nonce = nonce;
                    mine_success_client = client_ptr;
                    success = true;
                    break;
                }
            }
            if (success) {
                break;
            }
        }
    }
    // mine award
    clients[mine_success_client] += 6.25;
    // apply trx
    for (const auto &trx : pending_trxs) {
        std::string sender;
        std::string receiver;
        double value;
        parse_trx(trx, sender, receiver, value);
        clients[get_client(sender)] -= value;
        clients[get_client(receiver)] += value;
    }
    pending_trxs.clear();
    std::cout << mine_success_client->get_id() << std::endl;
    return success_nonce;
}