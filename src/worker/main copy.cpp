#include <cassert>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include <random>
#include <stdexcept>
#include <thread>
#include <kafka/KafkaClient.h>

void msg_consume(RdKafka::Message *message, void *opaque) {
  const RdKafka::Headers *headers;

  switch (message->err()) {
  case RdKafka::ERR__TIMED_OUT:
    std::cout << "timeout" << std::endl;
    break;

  case RdKafka::ERR_NO_ERROR:
    /* Real message */
    std::cout << "Read msg at offset " << message->offset() << std::endl;
    if (message->key()) {
      std::cout << "Key: " << *message->key() << std::endl;
    }
    headers = message->headers();
    if (headers) {
      std::vector<RdKafka::Headers::Header> hdrs = headers->get_all();
      for (size_t i = 0; i < hdrs.size(); i++) {
        const RdKafka::Headers::Header hdr = hdrs[i];

        if (hdr.value() != NULL)
          printf(" Header: %s = \"%.*s\"\n", hdr.key().c_str(),
                 (int)hdr.value_size(), (const char *)hdr.value());
        else
          printf(" Header:  %s = NULL\n", hdr.key().c_str());
      }
    }
    printf("%.*s\n", static_cast<int>(message->len()),
           static_cast<const char *>(message->payload()));
    break;

  case RdKafka::ERR__PARTITION_EOF:

  case RdKafka::ERR__UNKNOWN_TOPIC:
  case RdKafka::ERR__UNKNOWN_PARTITION:
    std::cerr << "Consume failed: " << message->errstr() << std::endl;
    exit(1);
    break;

  default:
    /* Errors */
    std::cerr << "Consume failed: " << message->errstr() << std::endl;
    exit(1);
  }
}

void set_conf(RdKafka::Conf *conf, const std::string &setting,
              const std::string &value) {
  std::string err;
  if (conf->set(setting, value, err) != RdKafka::Conf::CONF_OK) {
    throw std::runtime_error(err);
  }
}

int main() {

  std::string dbg;
  std::string err;

  const auto brokers = "localhost";
  const auto topic_str = "transcode_jobs";
  const auto groupid = "transcoders"; // Could also have been a logger keeping
                                      // track of transcoder jobs
  const auto partition = 0;
  const auto offset = 0; // Could be retrieved from worker SQL db to know which
                         // job was processed last

  auto conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  // Configuration

  set_conf(conf, "metadata.broker.list", brokers);
  set_conf(conf, "group.id", groupid);

  std::cout << "conf: " << std::endl;
  for (auto confItem : *conf->dump()) {
    std::cout << confItem.c_str() << ", ";
  }
  std::cout << std::endl;

  // Create consumer

  RdKafka::Consumer *consumer = RdKafka::Consumer::create(conf, err);
  if (!consumer) {
    std::cerr << err << std::endl;
    exit(1);
  }

  // Create topic handle
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  for (auto confItem : *tconf->dump()) {
    std::cout << confItem.c_str() << ", ";
  }
  std::cout << std::endl;

  RdKafka::Topic *topic =
      RdKafka::Topic::create(consumer, topic_str, tconf, err);
  if (!topic) {
    std::cerr << "Failed to create topic: " << err << std::endl;
    exit(1);
  }

  RdKafka::ErrorCode resp = consumer->start(topic, partition, offset);
  if (resp != RdKafka::ERR_NO_ERROR) {
    std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp)
              << std::endl;
    exit(1);
  }

  // /*
  //  * Consume messages
  //  */
  // while (true) {z
  //   RdKafka::Message *msg = consumer->consume(topic, partition, 1000);
  //   msg_consume(msg, NULL);
  //   delete msg;
  //   consumer->poll(0);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
  // }
  KAFKA_API::clients::KafkaClient::setGlobalLogger(0);
}