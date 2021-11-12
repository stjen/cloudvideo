#include "kafka/KafkaClient.h"
#include "kafka/KafkaConsumer.h"
#include "kafka/RdKafkaHelper.h"
#include "kafka/Types.h"
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <kafka_helpers/kafka_consumer.h>
#include <random>
#include <signal.h>
#include <stdexcept>
#include <thread>
#include <unistd.h>

std::atomic<bool> stopProducingFlag = false;

void got_signal(int) {
  std::cout << "Got stop signal!" << std::endl;
  stopProducingFlag.store(true);
}

int main(int argc, char **argv) {

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = got_signal;
  sigfillset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  const auto brokers = "10.20.20.30";
  kafka::Properties props({{"bootstrap.servers", brokers},
                           {"enable.auto.commit", "false"},
                           {"group.id", "transcoders"}});

// TODO: Only commit message when transcoding was successful
// Push hadoop URI to another log (??)
  auto jobCB = [](const kafka::clients::consumer::ConsumerRecord &record) -> bool {
    std::cout << "% Got a new message:," << std::endl;
    std::cout << "    Topic    : " << record.topic() << std::endl;
    std::cout << "    Partition: " << record.partition() << std::endl;
    std::cout << "    Offset   : " << record.offset() << std::endl;
    std::cout << "    Timestamp: " << record.timestamp().toString()
              << std::endl;
    std::cout << "    Headers  : " << kafka::toString(record.headers())
              << std::endl;
    std::cout << "    Key   [" << record.key().toString() << "]" << std::endl;
    std::cout << "    Value [" << record.value().toString() << "]" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 5 + 1));
    return (rand() % 3 == 1) ? false : true;
  };

  auto consumer =
      kafka_helpers::KafkaConsumer(jobCB, props, stopProducingFlag);
  consumer.startConsuming("transcode_jobs");
  while (!stopProducingFlag)
    ;
  std::cout << "Exiting" << std::endl;
  return 0;
}