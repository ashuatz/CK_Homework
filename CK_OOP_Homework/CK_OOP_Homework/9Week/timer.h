#pragma once
#include <thread>
#include <chrono>
#include <functional>

template <typename T>
class Timer
{
private:
	bool m_running = false;
	long m_interval;

	std::thread m_thread;
	std::function<T> m_func;

	bool m_initialized;
public:
	Timer() : m_initialized(false) {}

	Timer(const std::function<T>& func, const long &interval)
	{
		m_func = func;
		m_interval = interval;

		m_initialized = true;
	}

	void Initialize(const std::function<T>& func, const long &interval)
	{
		if (m_initialized)
		{
			stop();
		}

		m_func = func;
		m_interval = interval;

		m_initialized = true;
	}

	void start()
	{
		m_running = true;
		m_thread = std::thread([&]()
		{
			while (m_running)
			{
				auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_interval);

				//humm....
				m_func();
				std::this_thread::sleep_until(delta);
			}
		});
		m_thread.detach();
	}

	void stop() 
	{
		m_running = false;
		m_thread.~thread();
	}

	void restart() 
	{
		stop();
		start();
	}

	Timer& setFunc(std::function<void(void)> func) 
	{
		m_func = func;
		return *this;
	}

	Timer& setInterval(const long &interval)
	{
		m_interval = interval;
		return *this;
	}

	bool isInitialized() const
	{
		return m_initialized;
	}
	bool isRunning() const
	{
		return m_running;
	}

	long getInterval() const
	{
		return m_interval;
	}

	~Timer()
	{
		stop();
	}
};