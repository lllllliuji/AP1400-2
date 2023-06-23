#include "client.h"
#include <random>
#include "crypto.h"
#include "server.h"

Client::Client(std::string id, const Server &server) : server(&server), id(id) {
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const { return id; }

std::string Client::get_publickey() const { return public_key; }

double Client::get_wallet() const { return server->get_wallet(id); }

std::string Client::sign(std::string txt) const {
    std::string signature = crypto::signMessage(private_key, txt);
    return signature;
}

bool Client::transfer_money(std::string receiver, double value) {
    std::string sender = id;
    std::string trx = sender + "-" + receiver + "-" + std::to_string(value);
    auto signature = sign(trx);
    if (!server->add_pending_trx(trx, signature)) {
        return false;
    }
    return true;
}

size_t Client::generate_nonce() const {
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<size_t> u;
    return u(e);
}