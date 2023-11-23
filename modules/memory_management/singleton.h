
template<typename T>
class Singleton {

	static T * m_pSingleton;

public:
	Singleton() {
		assert(!m_pSingleton);
		m_pSingleton = static_cast<T*>(this);
	}

	~Singleton() {
		assert(m_pSingleton);
		m_pSingleton = nullptr;
	}

	static T& GetSingleton() {
		assert(m_pSignleton);
		return (*m_pSingleton);
	}

	static T* GetSingleton() {
		return m_pSingleton;
	}
};

template<typename T>
T * Singleton<T>::m_pSingleton = nullptr;