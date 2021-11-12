#include "kafka/Types.h"
#include <chrono>
#include <cstring>
#include <kafka/KafkaProducer.h>
#include <kafka_helpers/kafka_producer.h>
#include <signal.h>
#include <stdexcept>
#include <thread>
#include <unistd.h>
#include <util/atomic_queue.h>

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

  components::util::AtomicQueue<std::string> jobQueue;
  // Create configuration object
  const auto brokers = "10.20.20.30";
  kafka::Properties props({
      {"bootstrap.servers", {brokers}},
      {"enable.idempotence", "true"},
  });

  auto producer =
      kafka_helpers::KafkaProducer(jobQueue, props, stopProducingFlag);
  producer.startProducing("transcode_jobs");

  while (!stopProducingFlag) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50 + 500));
    auto job = std::to_string(rand() % 1000);
    std::cout << "sending: " << job << std::endl;
    jobQueue.push(job);
  }
}