#ifndef __MEMORY_SDERIALIZER__
#define __MEMORY_SDERIALIZER__
#include <windows.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <memory>
#include <cstddef>
#include <vector>
#include <algorithm>
namespace utility
{
	class binary_reader {
	private:
		uint64_t available_memory() {
			MEMORYSTATUSEX memory_status;
			memory_status.dwLength = sizeof(MEMORYSTATUSEX);
			if (GlobalMemoryStatusEx(&memory_status)) {
				return memory_status.ullAvailPhys;
			} else {
				throw std::runtime_error("Failed to retrieve memory status");
			}
			return 0;
		}
	public:
		explicit binary_reader() = default;
		~binary_reader() {
			if (m_blc != nullptr) {
				delete[] m_blc;
				m_blc = nullptr;
			} m_bytes = 0x0, m_mempos = 0;
		}
		explicit binary_reader(uint8_t* blc, const uintmax_t size) {
			if (size <= 0 || size >= UINT64_MAX || size >= available_memory())
				throw std::exception("invalid bytes to allocate");
			else {
				m_blc = new (std::nothrow) uint8_t[size];
				if (not m_blc)
					throw std::exception("not enough memory to allocate");
				// just buy more ram at this point lmao ><
				std::memcpy(m_blc, blc, size);
				m_bytes = size, m_mempos = 0;
			}
		}
		void file_to_bytes(const std::string& name, const std::string& encrypt_key = "") {
			std::ifstream file(name, std::ios::binary);
			if (file) {
				file.seekg(0, std::ios::end);
				size_t size = file.tellg();
				file.seekg(0, std::ios::beg);
				if (size > m_bytes) {
					resize(size);
				}
				file.read(reinterpret_cast<char*>(m_blc), size);
				file.close();
				if (!encrypt_key.empty()) {
					for (size_t i = 0; i < size; ++i) {
						m_blc[i] ^= encrypt_key[i % encrypt_key.size()];
					}
				}
			} else {
				throw std::exception("Failed to open file for reading");
			}
		}
		void resize(uintmax_t size) {
			if (m_bytes + size >= available_memory())
				throw std::exception("not enough memory to allocate");
			if (size > m_bytes) {
				uint8_t* blc = new (std::nothrow) uint8_t[size];
				if (blc == nullptr)
					throw std::exception("memory allocation failed");
				_STD memcpy(blc, m_blc, m_mempos);
				delete[] m_blc;
				m_blc = blc, m_bytes = size;
			}
		}
		void skip(uintmax_t _size) {
			if (_size != 0 and _size <= m_bytes)
				m_mempos += _size;
		}
		void cleanup() {
			if (m_blc != nullptr) {
				delete[] m_blc;
				m_blc = nullptr;
			} m_bytes = 0x0, m_mempos = 0;
		}
		void reverse() {
			if (m_blc == nullptr)
				throw std::exception("no memory allocated");
			std::reverse(m_blc, m_blc + m_bytes);
		}
		const bool check_bound(uintmax_t size) {
			if (size > m_bytes)
				return false;
			return true;
		}
		const std::string get_string() {
			if (check_bound(m_mempos + sizeof(uint16_t)) == false)
				throw std::exception("out of bound");
			uint16_t len = *reinterpret_cast<uint16_t*>(m_blc + m_mempos);
			m_mempos += sizeof(uint16_t);
			if (check_bound(m_mempos + len) == false)
				throw std::exception("out of bound");
			std::string str((char*)m_blc + m_mempos, len);
			m_mempos += len;
			return str;
		}
		const uint8_t get_ubytes() {
			if (check_bound(m_mempos + sizeof(uint8_t)) == false)
				throw std::exception("out of bound");
			uint8_t sum = *reinterpret_cast<uint8_t*>(m_blc + m_mempos);
			m_mempos += sizeof(uint8_t);
			return sum;
		}
		const uint16_t get_ushort() {
			if (check_bound(m_mempos + sizeof(uint16_t)) == false)
				throw std::exception("out of bound");
			uint16_t sum = *reinterpret_cast<uint16_t*>(m_blc + m_mempos);
			m_mempos += sizeof(uint16_t);
			return sum;
		}
		const uint32_t get_uint() {
			if (check_bound(m_mempos + sizeof(uint32_t)) == false)
				throw std::exception("out of bound");
			uint32_t sum = *reinterpret_cast<uint32_t*>(m_blc + m_mempos);
			m_mempos += sizeof(uint32_t);
			return sum;
		}
		const uint64_t get_ulong() {
			if (check_bound(m_mempos + sizeof(uint64_t)) == false)
				throw std::exception("out of bound");
			uint64_t sum = *reinterpret_cast<uint64_t*>(m_blc + m_mempos);
			m_mempos += sizeof(uint64_t);
			return sum;
		}
		const int8_t get_bytes() {
			if (check_bound(m_mempos + sizeof(int8_t)) == false)
				throw std::exception("out of bound");
			int8_t sum = *reinterpret_cast<int8_t*>(m_blc + m_mempos);
			m_mempos += sizeof(int8_t);
			return sum;
		}
		const int16_t get_short() {
			if (check_bound(m_mempos + sizeof(int16_t)) == false)
				throw std::exception("out of bound");
			int16_t sum = *reinterpret_cast<int16_t*>(m_blc + m_mempos);
			m_mempos += sizeof(int16_t);
			return sum;
		}
		const int32_t get_int() {
			if (check_bound(m_mempos + sizeof(int32_t)) == false)
				throw std::exception("out of bound");
			int32_t sum = *reinterpret_cast<int32_t*>(m_blc + m_mempos);
			m_mempos += sizeof(int32_t);
			return sum;
		}
		const int64_t get_long() {
			if (check_bound(m_mempos + sizeof(int64_t)) == false)
				throw std::exception("out of bound");
			int64_t sum = *reinterpret_cast<int64_t*>(m_blc + m_mempos);
			m_mempos += sizeof(int64_t);
			return sum;
		}
		const float get_float() {
			if (check_bound(m_mempos + sizeof(float)) == false)
				throw std::exception("out of bound");
			float sum = *reinterpret_cast<float*>(m_blc + m_mempos);
			m_mempos += sizeof(float);
			return sum;
		}
		const double get_double() {
			if (check_bound(m_mempos + sizeof(double)) == false)
				throw std::exception("out of bound");
			double sum = *reinterpret_cast<double*>(m_blc + m_mempos);
			m_mempos += sizeof(double);
			return sum;
		}
		const bool get_bool() {
			if (check_bound(m_mempos + sizeof(bool)) == false)
				throw std::exception("out of bound");
			bool sum = *reinterpret_cast<bool*>(m_blc + m_mempos);
			m_mempos += sizeof(bool);
			return sum;
		}
		constexpr uint8_t* get() {
			return m_blc != nullptr ? m_blc : nullptr;
		}
		constexpr uintmax_t size() {
			return m_bytes;
		}
		constexpr uintmax_t mempos() {
			return m_mempos;
		}
	private:
		uint8_t* m_blc = NULL;
	private:
		uintmax_t m_mempos = 0;
		uintmax_t m_bytes = 0; 
	};

	class binary_writer {
	private:
		uint64_t available_memory() {
			MEMORYSTATUSEX memory_status;
			memory_status.dwLength = sizeof(MEMORYSTATUSEX);
			if (GlobalMemoryStatusEx(&memory_status)) {
				return memory_status.ullAvailPhys;
			} else {
				throw std::runtime_error("Failed to retrieve memory status");
			}
			return 0;
		}
	public:
		explicit binary_writer() = default;
		~binary_writer() {
			if (m_blc != nullptr) {
				delete[] m_blc;
				m_blc = nullptr;
			} m_bytes = 0x0, m_mempos = 0;
		}
		explicit binary_writer(const uintmax_t size) {
			if (size <= 0 || size >= UINT64_MAX || size >= available_memory())
				throw std::exception("invalid bytes to allocate");
			else {
				m_blc = new (std::nothrow) uint8_t[size];
				if (not m_blc)
					throw std::exception("not enough memory to allocate");
				std::memset(m_blc, 0x0, size);
				m_bytes = size, m_mempos = 0;
			}
		}
		const bool check_bound(uintmax_t size) {
			if (size > m_bytes)
				return false;
			return true;
		}
		void set_string(const std::string& str) {
			uint16_t len = static_cast<uint16_t>(str.size());
			if (check_bound(m_mempos + sizeof(uint16_t) + len) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &len, sizeof(uint16_t));
			m_mempos += sizeof(uint16_t);
			std::memcpy(m_blc + m_mempos, str.data(), len);
			m_mempos += len;
		}
		void set_ubytes(const uint8_t sum) {
			if (check_bound(m_mempos + sizeof(uint8_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(uint8_t));
			m_mempos += sizeof(uint8_t);
		}
		void set_ushort(const uint16_t sum) {
			if (check_bound(m_mempos + sizeof(uint16_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(uint16_t));
			m_mempos += sizeof(uint16_t);
		}
		void set_uint(const uint32_t sum) {
			if (check_bound(m_mempos + sizeof(uint32_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(uint32_t));
			m_mempos += sizeof(uint32_t);
		}
		void set_ulong(const uint64_t sum) {
			if (check_bound(m_mempos + sizeof(uint64_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(uint64_t));
			m_mempos += sizeof(uint64_t);
		}
		void set_bytes(const int8_t sum) {
			if (check_bound(m_mempos + sizeof(int8_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(int8_t));
			m_mempos += sizeof(int8_t);
		}
		void set_short(const int16_t sum) {
			if (check_bound(m_mempos + sizeof(int16_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(int16_t));
			m_mempos += sizeof(int16_t);
		}
		void set_int(const int32_t sum) {
			if (check_bound(m_mempos + sizeof(int32_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(int32_t));
			m_mempos += sizeof(int32_t);
		}
		void set_long(const int64_t sum) {
			if (check_bound(m_mempos + sizeof(int64_t)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(int64_t));
			m_mempos += sizeof(int64_t);
		}
		void set_float(const float sum) {
			if (check_bound(m_mempos + sizeof(float)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(float));
			m_mempos += sizeof(float);
		}
		void set_double(const double sum) {
			if (check_bound(m_mempos + sizeof(double)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(double));
			m_mempos += sizeof(double);
		}
		void set_bool(const bool sum) {
			if (check_bound(m_mempos + sizeof(bool)) == false)
				throw std::exception("out of bound");
			std::memcpy(m_blc + m_mempos, &sum, sizeof(bool));
			m_mempos += sizeof(bool);
		}
		void skip(uintmax_t _size) {
			if (_size != 0 and _size <= m_bytes)
				m_mempos += _size;
		}
		void cleanup() {
			if (m_blc != nullptr) {
				delete[] m_blc;
				m_blc = nullptr;
			} m_bytes = 0x0, m_mempos = 0;
		}
		void reverse() {
			if (m_blc == nullptr)
				throw std::exception("no memory allocated");
			std::reverse(m_blc, m_blc + m_bytes);
		}
		void resize(uintmax_t size) {
			if (m_bytes + size >= available_memory())
				throw std::exception("not enough memory to allocate");
			if (size > m_bytes) {
				uint8_t* blc = new (std::nothrow) uint8_t[size];
				if (blc == nullptr)
					throw std::exception("memory allocation failed");
				_STD memcpy(blc, m_blc, m_mempos);
				delete[] m_blc;
				m_blc = blc, m_bytes = size;
			}
		}
		void bytes_to_file(const std::string& name, const std::string& encrypt_key = "") {
			if (encrypt_key.empty()) {
				std::ofstream file(name, std::ios::binary);
				if (file) {
					file.write(reinterpret_cast<const char*>(m_blc), m_mempos);
					file.close();
				} else {
					throw std::exception("Failed to open file for writing");
				}
			} else {
				std::ofstream file(name, std::ios::binary);
				if (file) {
					for (size_t i = 0; i < m_mempos; ++i) {
						m_blc[i] ^= encrypt_key[i % encrypt_key.size()];
					}
					file.write(reinterpret_cast<const char*>(m_blc), m_mempos);
					file.close();
				} else {
					throw std::exception("Failed to open file for writing");
				}
			}
		}
		void file_to_bytes(const std::string& name, const std::string& encrypt_key = "") {
			std::ifstream file(name, std::ios::binary);
			if (file) {
				file.seekg(0, std::ios::end);
				size_t size = file.tellg();
				file.seekg(0, std::ios::beg);
				if (size > m_bytes) {
					resize(size);
				}
				file.read(reinterpret_cast<char*>(m_blc), size);
				file.close();
				if (!encrypt_key.empty()) {
					for (size_t i = 0; i < size; ++i) {
						m_blc[i] ^= encrypt_key[i % encrypt_key.size()];
					}
				}
			} else {
				throw std::exception("Failed to open file for reading");
			}
		}
		constexpr uint8_t* get() {
			return m_blc != nullptr ? m_blc : nullptr;
		}
		constexpr uintmax_t size() {
			return m_bytes;
		}
		constexpr uintmax_t mempos() {
			return m_mempos;
		}
	private:
		uint8_t* m_blc = NULL;
	private:
		uintmax_t m_mempos = 0; 
		uintmax_t m_bytes = 0;
	};
}
#endif // !__MEMORY_SDERIALIZER__
