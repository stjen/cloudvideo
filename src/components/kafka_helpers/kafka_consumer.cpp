#include "kafka/KafkaConsumer.h"
#include "kafka/Types.h"
#include "kafka_consumer.h"
#include <chrono>
#include <iostream>
#include <thread>

using namespace kafka_helpers;

KafkaConsumer::KafkaConsumer(KafkaRecordCB jobCb, kafka::Properties properties,
                             std::atomic<bool> &stopConsumingFlag)
    : m_stopConsumingFlag(stopConsumingFlag), m_jobCb(jobCb) {

  m_consumer = std::make_unique<kafka::clients::KafkaConsumer>(properties);
}

void KafkaConsumer::startConsuming(const std::string &topic) {
  m_consumerThread = std::thread(&KafkaConsumer::consumerThread, this, topic);
}
void KafkaConsumer::consumerThread(const std::string &topic) {
  std::cout << "Starting job consuming" << std::endl;

  try {

    auto lastTimeCommitted = std::chrono::steady_clock::now();
    bool pendingCommit = false;
    auto pendingSuccessfulJobs = kafka::TopicPartitionOffsets();
    bool failedProcessing = false;

    // Subscribe to topics
    m_consumer->subscribe({topic});

    // Read messages from the topic.
    std::cout << "% Reading messages from topic: " << topic << std::endl;
    while (!m_stopConsumingFlag) {
      auto records = m_consumer->poll(std::chrono::milliseconds(100));
      std::cout << "recordsize: " << records.size() << std::endl;
      for (const auto &record : records) {
        if (m_stopConsumingFlag)
          break;
        if (record.value().size() == 0)
          std::cout << "Got empty message";

        if (!record.error()) {

          pendingSuccessfulJobs.insert_or_assign(
              kafka::TopicPartition(record.topic(), record.partition()),
              record.offset());
          if (m_jobCb(record)) {
            std::cout << "offset1: " << record.offset() << std::endl;
            pendingCommit = true;
          } else {
            std::cout << "oops\n\n";
            failedProcessing = true;
            break;
          }

        } else {
          std::cerr << record.toString() << std::endl;
        }
      }
      std::cout << "out" << std::endl;
      if (pendingCommit) {
        auto now = std::chrono::steady_clock::now();
        if ((std::chrono::steady_clock::now() - lastTimeCommitted >
             std::chrono::seconds(COMMIT_TIMEOUT)) ||
            failedProcessing) {
          // Commit offsets for messages polled that were succesfully processed
          std::cout << "offset2: " << pendingSuccessfulJobs.begin()->second
                    << std::endl;
          m_consumer->commitSync(pendingSuccessfulJobs); // or commitAsync()
          pendingSuccessfulJobs.clear();

          lastTimeCommitted = now;
          pendingCommit = false;
        }
      }
      if (failedProcessing) {
        std::cout << "jobCb returned false, rewinding and not committing"
                  << std::endl;
        failedProcessing = false;
        continue;
      }
    }
    std::cout << "Stopped consuming " << std::endl;

  } catch (const kafka::KafkaException &e) {
    std::cerr << "% Unexpected exception caught: " << e.what() << std::endl;
  }
  /*

  */
}