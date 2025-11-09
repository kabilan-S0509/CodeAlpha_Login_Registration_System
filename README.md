# CodeAlpha_Login_Registration_System
# CodeAlpha C++ Internship Project  
## Project: Login and Registration System  

### 📘 Description
A secure C++ console application that allows users to **register**, **login**, and **reset passwords**.  
Each user’s credentials are stored safely using a salted hash (not plain text).

---

### ⚙️ Features
- Register new users with username & password  
- Validates strong password and unique username  
- Stores credentials securely in one file per user (`username.usr`)  
- Login verification with hashed passwords  
- Password reset option  

---

### 💻 How to Run
```bash
g++ Login_Registration_System.cpp -o LoginSystem
./LoginSystem   # (Windows: .\LoginSystem)
