# 🚀 API & Frontend Guide

> A beginner-friendly guide to understanding how your project connects frontend to backend.

---

## 📦 What is an API?

**API** stands for **Application Programming Interface**.

Think of it like ordering food:

| Role | Your Project |
|------|--------------|
| 🧑 You (customer) | Frontend (browser) |
| 🍽️ Waiter | API |
| 👨‍🍳 Kitchen | Backend (C++ server) |

You tell the waiter what you want → Waiter tells the kitchen → Kitchen makes it → Waiter brings it back.

---

## 📋 Your APIs

### Customer APIs

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/login` | POST | Login with ID + password |
| `/api/register` | POST | Create new account |
| `/api/profile` | GET | Get customer profile |

### Product APIs

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/product` | GET | Get all products |
| `/api/product` | POST | Search product by name |
| `/api/product/sorted` | GET | Get products sorted by price ⭐ |

### Cart APIs

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/cart` | GET | View cart items |
| `/api/cart/add` | POST | Add item to cart |
| `/api/cart/remove` | POST | Remove item from cart |
| `/api/cart/checkout` | POST | Complete purchase |

### Admin APIs

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/admin/login` | POST | Admin login |
| `/api/admin/addProduct` | POST | Add new product |
| `/api/admin/deleteProduct` | POST | Delete product |

---

## 🔄 GET vs POST

| | GET | POST |
|--|-----|------|
| **Purpose** | Read data | Send data |
| **Example** | Get product list | Login with password |
| **Body** | No data sent | Contains data |

---

## 🔗 How JavaScript Talks to the Server

JavaScript uses `fetch()` to communicate with APIs.

### Basic Pattern

```javascript
// 1️⃣ Call the API
const response = await fetch('/api/product');

// 2️⃣ Convert to JavaScript object
const data = await response.json();

// 3️⃣ Use the data
console.log(data.products);
```

---

## 📖 Examples

### Example 1: Get Products

```javascript
async function loadProducts() {
    const response = await fetch('/api/product');
    const data = await response.json();
    
    if (data.success) {
        data.products.forEach(p => {
            console.log(p.name, p.price);
        });
    }
}
```

### Example 2: Sort Products by Price

```javascript
async function sortProductsByPrice() {
    const response = await fetch('/api/product/sorted');
    const data = await response.json();
    
    // Products are now sorted: cheapest first!
    displayProducts(data.products);
}
```

### Example 3: Login (Sending Data)

```javascript
async function handleLogin() {
    const response = await fetch('/api/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            id: 123,
            password: "mypassword"
        })
    });
    
    const data = await response.json();
    
    if (data.exists) {
        alert("Login successful!");
    }
}
```

---

## 🔀 The Sort Feature Flow

```
1. User clicks "Sort by Price" button
              ↓
2. JavaScript calls fetch('/api/product/sorted')
              ↓
3. C++ server receives request (main.cpp)
              ↓
4. Quick Sort algorithm runs (ProductBST.cpp)
              ↓
5. Sorted products returned as JSON
              ↓
6. JavaScript displays sorted products
```

**Files involved:**

| File | Role |
|------|------|
| `product.html` | Button that user clicks |
| `script.js` | Calls the API with fetch() |
| `main.cpp` | API endpoint `/api/product/sorted` |
| `ProductBST.cpp` | Quick Sort algorithm |

---

## 📄 JSON Format

JSON is the data format used for communication.

```json
{
    "success": true,
    "products": [
        { "name": "Apple", "price": 1.50 },
        { "name": "Bread", "price": 2.00 }
    ]
}
```

**Access in JavaScript:**
```javascript
data.success           // true
data.products[0].name  // "Apple"
data.products[0].price // 1.50
```

---

## 📝 Quick Reference

| Term | Meaning |
|------|---------|
| Frontend | HTML, CSS, JavaScript (what user sees) |
| Backend | C++ server (processes requests) |
| API | URL endpoint that accepts requests |
| fetch() | JavaScript function to call APIs |
| JSON | Data format for communication |
| GET | Request to read data |
| POST | Request to send data |
