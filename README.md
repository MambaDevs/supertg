#Supertg
Fast and Portable Cryptography Extension Library for Python

Supertg is a high-performance cryptography library written in C++ as a Python extension. It is designed to be portable, efficient, and easy to integrate into Python projects. SuperCrypto provides implementations of essential cryptographic algorithms, commonly used in secure communications, including:

AES-256-IGE: For encryption in protocols like MTProto v2.0.
AES-256-CTR: For stream-based encryption (e.g., CDN encrypted files).
AES-256-CBC: For block-based encryption (e.g., passport credentials).
Requirements
Python 3.7 or higher.
A compiler supporting C++17 or later.
Installation
Install Supertg via PyPI:

`bash
pip install Supertg`

__API__
The Supertg library provides the following API:

`python
def ige256_encrypt(data: bytes, key: bytes, iv: bytes) -> bytes: ...
def ige256_decrypt(data: bytes, key: bytes, iv: bytes) -> bytes: ...

def ctr256_encrypt(data: bytes, key: bytes, iv: bytes, state: bytes) -> bytes: ...
def ctr256_decrypt(data: bytes, key: bytes, iv: bytes, state: bytes) -> bytes: ...

def cbc256_encrypt(data: bytes, key: bytes, iv: bytes) -> bytes: ...
def cbc256_decrypt(data: bytes, key: bytes, iv: bytes) -> bytes: ...`

Usage Examples
IGE Mode
`python
Copy code
import os
import Supertg`

data = os.urandom(10 * 1024 * 1024 + 7)  # 10 MB of random data + 7 bytes to show padding
key = os.urandom(32)  # Random Key
iv = os.urandom(32)  # Random IV

# Pad with zeroes: -7 % 16 = 9
data += bytes(-len(data) % 16)

ige_encrypted = Supertg.ige256_encrypt(data, key, iv)
ige_decrypted = Supertg.ige256_decrypt(ige_encrypted, key, iv)

print(data == ige_decrypted)  # True
CTR Mode (Single Chunk)
python
Copy code
import os
import Supertg

data = os.urandom(10 * 1024 * 1024)  # 10 MB of random data
key = os.urandom(32)  # Random Key

enc_iv = bytearray(os.urandom(16))  # Random IV
dec_iv = enc_iv.copy()  # Keep a copy for decryption

ctr_encrypted = Supertg.ctr256_encrypt(data, key, enc_iv, bytes(1))
ctr_decrypted = Supertg.ctr256_decrypt(ctr_encrypted, key, dec_iv, bytes(1))

print(data == ctr_decrypted)  # True
CBC Mode
python
Copy code
import os
import Supertg

data = os.urandom(10 * 1024 * 1024 + 7)  # 10 MB of random data + 7 bytes to show padding
key = os.urandom(32)  # Random Key
iv = os.urandom(16)  # Random IV

# Pad with zeroes: -7 % 16 = 9
data += bytes(-len(data) % 16)

cbc_encrypted = Supertg.cbc256_encrypt(data, key, iv)
cbc_decrypted = Supertg.cbc256_decrypt(cbc_encrypted, key, iv)

print(data == cbc_decrypted)  # True
Testing
To run tests for Supertg:

Clone the repository:
bash
Copy code
git clone https://github.com/mambadevs/Supertg
Navigate to the directory:
bash
Copy code
cd Supertg
Install tox:
bash
Copy code
pip install tox
Run tests:
bash
Copy code
tox
Features
Cross-platform support: Works seamlessly on Windows, macOS, and Linux.
Performance-optimized: Written in C++ for maximum speed.
Easy integration: Installable via pip with no extra dependencies.
License
Supertg is released under the MIT License.

