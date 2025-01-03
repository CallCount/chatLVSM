#include "encrypt.h"

std::string Encrypt::encrypt(std::string& msg, std::string& key) {
  std::vector<char> msg2(msg.begin(), msg.end());
  std::string b64_str = Base64::encode(msg2);
  std::string vigenere_msg = Vigenere::encrypt_vigenere(b64_str, key);
  return vigenere_msg;
}

std::string Encrypt::sanitize_utf8(std::string& str) {
  int i, f_size = str.size();
  unsigned char c, c2, c3, c4;
  std::string to;
  to.reserve(f_size);

  for (i = 0; i < f_size; i++) {
    c = (unsigned char)(str)[i];
    if (c < 32) {
      if (c == 9 || c == 10 || c == 13) {
        to.append(1, c);
      }
      continue;
    } else if (c < 127) {
      to.append(1, c);
      continue;
    } else if (c < 160) {
      if (c2 == 128) {
        to.append(1, 226);
        to.append(1, 130);
        to.append(1, 172);
      }
      if (c2 == 133) {
        to.append(1, 10);
        to.append(1, 13);
      }
      continue;
    } else if (c < 192) {
      to.append(1, (unsigned char)194);
      to.append(1, c);
      continue;
    } else if (c < 194) {
      to.append(1, (unsigned char)195);
      to.append(1, c - 64);
      continue;
    } else if (c < 224 && i + 1 < f_size) {
      c2 = (unsigned char)(str)[i + 1];
      if (c2 > 127 && c2 < 192) {
        if (c == 194 && c2 < 160) {
          ;
        } else {
          to.append(1, c);
          to.append(1, c2);
        }
        i++;
        continue;
      }
    } else if (c < 240 && i + 2 < f_size) {
      c2 = (unsigned char)(str)[i + 1];
      c3 = (unsigned char)(str)[i + 2];
      if (c2 > 127 && c2 < 192 && c3 > 127 && c3 < 192) {
        to.append(1, c);
        to.append(1, c2);
        to.append(1, c3);
        i += 2;
        continue;
      }
    } else if (c < 245 && i + 3 < f_size) {
      c2 = (unsigned char)(str)[i + 1];
      c3 = (unsigned char)(str)[i + 2];
      c4 = (unsigned char)(str)[i + 3];
      if (c2 > 127 && c2 < 192 && c3 > 127 && c3 < 192 && c4 > 127 &&
          c4 < 192) {
        to.append(1, c);
        to.append(1, c2);
        to.append(1, c3);
        to.append(1, c4);
        i += 3;
        continue;
      }
    }
    to.append(1, (unsigned char)195);
    to.append(1, c - 64);
  }
  return to;
}

std::string Encrypt::decrypt(std::string& encrypted_msg, std::string& key) {
  std::string newKey = Vigenere::extend_key(encrypted_msg, key);
  std::string b64_encoded_str =
      Vigenere::decrypt_vigenere(encrypted_msg, newKey);
  std::vector<char> b64_decode_vec = Base64::decode(b64_encoded_str);
  std::string b64_decode_str(b64_decode_vec.begin(), b64_decode_vec.end());
  return sanitize_utf8(b64_decode_str);
}