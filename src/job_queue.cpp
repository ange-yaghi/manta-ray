#include <job_queue.h>

manta::JobQueue::JobQueue() {

}

manta::JobQueue::~JobQueue() {

}

void manta::JobQueue::push(const Job &job) {
	m_queueLock.lock();
	m_queue.push(job);
	m_queueLock.unlock();
}

bool manta::JobQueue::pop(Job *job) {
	bool result;

	m_queueLock.lock();
	if (m_queue.empty()) {
		result = false;
	}
	else {
		*job = m_queue.front();
		m_queue.pop();
		m_queueLock.unlock();

		result = true;
	}

	return result;
}
