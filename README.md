# 🛒 E-Commerce Platform with Data Structures

A full-stack e-commerce web application built with **C++ backend** and **HTML/CSS/JavaScript frontend**, demonstrating practical implementation of various data structures including Binary Search Trees, Queues, Stacks, and Linked Lists.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=c%2B%2B)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=flat-square&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=flat-square&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=flat-square&logo=javascript&logoColor=black)

---

## 📋 Table of Contents

- [Features](#-features)
- [Data Structures Used](#-data-structures-used)
- [Project Structure](#-project-structure)
- [Prerequisites](#-prerequisites)
- [Installation & Setup](#-installation--setup)
- [Running the Application](#-running-the-application)
- [API Endpoints](#-api-endpoints)
- [Screenshots](#-screenshots)
- [Technologies Used](#-technologies-used)
- [Contributors](#-contributors)

---

## ✨ Features

### Customer Features
- 🔐 **User Authentication** - Secure login and registration system
- 🛍️ **Product Browsing** - Browse products by category and provider
- 🔍 **Product Search** - Search products using Binary Search Tree
- 📊 **Product Sorting** - Sort products by price using Quick Sort algorithm
- 🛒 **Shopping Cart** - Add, remove, and manage cart items
- 💳 **Checkout System** - Complete orders with Cash or Visa payment
- 📦 **Order History** - View past orders using Stack data structure
- 👤 **User Profile** - View and manage personal information

### Admin/Provider Features
- 🔑 **Provider Login** - Separate authentication for providers
- ➕ **Add Products** - Add new products to inventory
- 🗑️ **Delete Products** - Remove products from catalog
- 📋 **Order Management** - View and process pending orders using Queue
- 🚚 **Delivery Processing** - Dequeue and ship orders

### General Features
- 🌙 **Dark/Light Theme** - Toggle between themes with smooth transitions
- 📱 **Responsive Design** - Works on desktop and mobile devices
- 💾 **Persistent Data** - All data stored in CSV files

---

## 🏗️ Data Structures Used

| Data Structure | Implementation | Purpose |
|----------------|----------------|---------|
| **Binary Search Tree (BST)** | `ProductBST` | Product storage and efficient search operations |
| **Queue** | `DeliveryQueue` | Managing delivery orders (FIFO) |
| **Stack** | `OrderHistoryStack` | Storing customer order history (LIFO - newest first) |
| **Doubly Linked List** | `CartList` | Shopping cart implementation |
| **Singly Linked List** | `Customer_Accounts` | Customer account management |

### Algorithms Implemented
- **Quick Sort** - Sorting products by price (O(n log n) average)
- **BST Operations** - Insert, Delete, Search (O(log n) average)
- **Queue Operations** - Enqueue, Dequeue (O(1))
- **Stack Operations** - Push, Pop, Top (O(1))

---

## 📁 Project Structure

```
Data-Structure-Project/
├── 📂 src/                      # C++ Source Files
│   ├── main.cpp                 # Main server with API endpoints
│   ├── ProductBST.cpp           # BST implementation
│   ├── DeliveryQueue.cpp        # Queue implementation
│   ├── cart.cpp                 # Cart linked list implementation
│   ├── orderhistory.cpp         # Stack implementation
│   ├── CustomerAccounts.cpp     # Customer linked list
│   └── server.cpp               # Server utilities
│
├── 📂 include/                  # Header Files
│   ├── ProductBST.h             # Product & BST class definitions
│   ├── DeliveryQueue.h          # Order & Queue class definitions
│   ├── cart.h                   # CartItem, CartNode, Cart classes
│   ├── orderhistory.h           # OrderHistoryStack class
│   ├── CustomerAccounts.h       # Customer accounts class
│   ├── customer.h               # Customer class definition
│   ├── server.h                 # Server class definition
│   ├── httplib.h                # HTTP library (cpp-httplib)
│   ├── json.hpp                 # JSON library (nlohmann/json)
│   └── rapidcsv.h               # CSV parsing library
│
├── 📂 frontend/                 # Frontend Files
│   ├── home.html                # Home page
│   ├── login.html               # Login page
│   ├── signup.html              # Registration page
│   ├── product.html             # Product catalog page
│   ├── cart.html                # Shopping cart page
│   ├── payment.html             # Payment/checkout page
│   ├── profile.html             # User profile page
│   ├── providers.html           # Providers listing page
│   ├── admin.html               # Admin dashboard
│   ├── script.js                # Main JavaScript file
│   └── 📂 images/               # Image assets
│
├── 📂 data/                     # CSV Data Files
│   ├── Customer_List.csv        # Customer accounts
│   ├── Products.csv             # Product catalog
│   ├── Orders.csv               # Order history
│   ├── Carts.csv                # Cart data
│   └── Providers.csv            # Provider information
│
├── app.exe                      # Compiled executable
└── README.md                    # This file
```

---

## 📋 Prerequisites

Before running the project, ensure you have:

- **G++ Compiler** (MinGW recommended for Windows)
  - Supports C++17 or later
- **Web Browser** (Chrome, Firefox, Edge, etc.)
- **Windows OS** (tested on Windows 10/11)

---

## 🚀 Installation & Setup

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/Data-Structure-Project.git
cd Data-Structure-Project
```

### 2. Compile the Project
```bash
g++ -o app.exe src/main.cpp src/ProductBST.cpp src/server.cpp src/DeliveryQueue.cpp src/cart.cpp src/orderhistory.cpp src/CustomerAccounts.cpp -lws2_32 -std=c++17
```

> **Note:** The `-lws2_32` flag is required for Windows socket support (used by httplib).

---

## ▶️ Running the Application

### 1. Start the Server
```bash
./app.exe
```

You should see:
```
Initialized globalOrderID to XXXX
Server started! Open http://localhost:9090/login.html in your browser
```

### 2. Open in Browser
Navigate to: **http://localhost:9090/login.html**

### 3. Test Accounts

#### Customer Login
| ID | Password |
|----|----------|
| 1939 | 1939 |
| 241000245 | 123 |

#### Provider/Admin Login
| Username | Password |
|----------|----------|
| Bazooka | 123 |
| El Abd | 123 |
| Lampatronics | 123 |
| B Tech | 123 |

---

## 🔌 API Endpoints

### Authentication
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/login` | Customer login |
| POST | `/api/register` | Customer registration |
| POST | `/api/admin/login` | Provider/Admin login |

### Products
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/product` | Get all products |
| GET | `/api/product?provider=X` | Get products by provider |
| GET | `/api/product/sorted` | Get products sorted by price (Quick Sort) |
| POST | `/api/product` | Search product by name |

### Cart
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/cart?customerId=X` | Get cart items |
| POST | `/api/cart/add` | Add item to cart |
| POST | `/api/cart/remove` | Remove item from cart |
| POST | `/api/cart/clear` | Clear entire cart |
| POST | `/api/cart/checkout` | Checkout cart |

### Orders
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/orders/:id` | Get customer order history |
| GET | `/api/admin/deliveryQueue` | Get pending orders (Queue) |
| POST | `/api/admin/processOrder` | Process next order (Dequeue) |

### Profile & Providers
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/profile` | Get customer profile |
| GET | `/api/providers` | Get all providers |

### Admin Operations
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/admin/addProduct` | Add new product |
| POST | `/api/admin/deleteProduct` | Delete a product |

---

## 🛠️ Technologies Used

### Backend
- **C++17** - Core programming language
- **[cpp-httplib](https://github.com/yhirose/cpp-httplib)** - HTTP server library
- **[nlohmann/json](https://github.com/nlohmann/json)** - JSON parsing
- **[rapidcsv](https://github.com/d99kris/rapidcsv)** - CSV file handling

### Frontend
- **HTML5** - Page structure
- **CSS3** - Styling with glassmorphism effects
- **JavaScript** - Client-side logic and API calls
- **Fetch API** - HTTP requests

### Data Storage
- **CSV Files** - Persistent data storage

---

## 🎨 UI Features

- **Glassmorphism Design** - Modern frosted glass effect
- **Gradient Themes** - Blue gradient color scheme
- **Dark/Light Mode** - Toggle between themes
- **Smooth Animations** - Page transitions and hover effects
- **Responsive Layout** - Mobile-friendly design

---

## 📚 Learning Outcomes

This project demonstrates:

1. **Data Structure Implementation**
   - Building BST from scratch for product management
   - Queue implementation for order processing
   - Stack for order history with LIFO access
   - Linked lists for cart and customer management

2. **Algorithm Application**
   - Quick Sort for price-based sorting
   - BST traversal for product retrieval
   - Efficient search operations

3. **Full-Stack Development**
   - RESTful API design
   - Client-server architecture
   - Frontend-backend communication

4. **File I/O Operations**
   - Reading and writing CSV files
   - Data persistence

---

## 👥 Contributors
Mariam Gamal
Mariam Elbadry
Kareem Gamal
Yahya Haitham

---

## 📄 License

This project is developed for educational purposes as part of a Data Structures course.

---

## 🙏 Acknowledgments

- [cpp-httplib](https://github.com/yhirose/cpp-httplib) - Lightweight HTTP library
- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++
- [rapidcsv](https://github.com/d99kris/rapidcsv) - Fast CSV parser

---

<p align="center">
  Made with ❤️ for Data Structures Course
</p>
