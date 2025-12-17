// This file now communicates with the C++ backend via API calls.
console.log("script.js loaded successfully");

const API_ENDPOINTS = {
    LOGIN: '/api/login',
    REGISTER: '/api/register',
    PROFILE: '/api/profile',
    PRODUCT: '/api/product',
    ADMIN_QUEUE: '/api/admin/deliveryQueue',
    ADMIN_PROCESS: '/api/admin/processOrder',
    PROVIDERS: '/api/providers'
};

async function handleRegistration() {
    const id = document.getElementById("id").value;
    const name = document.getElementById("name").value;
    const phone = document.getElementById("phone").value;
    const address = document.getElementById("address").value;
    const password = document.getElementById("password").value;
    const confirmPassword = document.getElementById("confirm_password").value;

    if (!id || !name || !phone || !address || !password || !confirmPassword) {
        alert("Please fill in all fields.");
        return;
    }

    if (password !== confirmPassword) {
        alert("Passwords do not match!");
        return;
    }

    try {
        const response = await fetch(API_ENDPOINTS.REGISTER, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                id: parseInt(id),
                name: name,
                phone: phone,
                address: address,
                password: password,
                confirmPassword: confirmPassword
            })
        });

        const data = await response.json();

        if (response.ok && data.success) {
            alert("Registration successful! Please login.");
            window.location.href = "login.html";
        } else {
            alert(`Registration failed: ${data.message || 'Unknown error'}`);
        }
    } catch (error) {
        console.error('Error during registration:', error);
        alert('An error occurred. Please try again.');
    }
}

// Login / Sign up function
// async function login() {
//     const id = document.getElementById("id").value;
//     const name = document.getElementById("name").value;
//     const phone = document.getElementById("phone").value;
//     const address = document.getElementById("address").value;
//     const password = document.getElementById("password").value;
//     const confirmPassword = document.getElementById("confirm_password").value;

//     if (!id) {
//         alert("Customer ID is required.");
//         return;
//     }

//     // If name, phone, and address are provided, it's a registration attempt
//     if (name && phone && address && password && confirmPassword) {
//         try {
//             const response = await fetch(API_ENDPOINTS.REGISTER, {
//                 method: 'POST',
//                 headers: { 'Content-Type': 'application/json' },
//                 body: JSON.stringify({
//                     id: parseInt(id),
//                     name: name,
//                     phone: phone,
//                     address: address,
//                     password: password,
//                     confirmPassword: confirmPassword
//                 })
//             });

//             const data = await response.json();

//             if (response.ok && data.success) {
//                 alert("Registration successful! Redirecting to profile.");
//                 localStorage.setItem("currentCustomerId", id);
//                 window.location.href = "profile.html";
//             } else {
//                 alert(`Registration failed: ${data.message || 'Unknown error'}`);
//             }
//         } catch (error) {
//             console.error('Error during registration:', error);
//             alert('An error occurred. Please try again.');
//         }
//     } else {
//         // Otherwise, it's a login attempt
//         try {
//             const response = await fetch(API_ENDPOINTS.LOGIN, {
//                 method: 'POST',
//                 headers: { 'Content-Type': 'application/json' },
//                 body: JSON.stringify({ id: parseInt(id) })
//             });

//             const data = await response.json();

//             if (response.ok && data.exists) {
//                 localStorage.setItem("customerId", id);
//                 window.location.href = "profile.html";
//             } else {
//                 alert("Login failed: Customer ID not found. Please fill in all fields to register.");
//             }
//         } catch (error) {
//             console.error('Error during login:', error);
//             alert('An error occurred. Please try again.');
//         }
//     }
// }

// Load profile data
async function loadProfile() {
    const customerId = localStorage.getItem("customerId");
    const password = localStorage.getItem("customerPassword");

    if (!customerId) {
        window.location.href = "login.html";
        return;
    }

    try {
        const response = await fetch(`${API_ENDPOINTS.PROFILE}?id=${customerId}&password=${password}`);
        if (response.ok) {
            const customer = await response.json();
            document.getElementById("profile").innerHTML = `
                <b>ID:</b> ${customer.id}<br>
                <b>Name:</b> ${customer.name}<br>
                <b>Phone:</b> ${customer.phone}<br>
                <b>Address:</b> ${customer.address}
            `;
        } else {
            alert(`Failed to load profile: ${await response.text()}`);
            localStorage.removeItem("currentCustomerId");
            window.location.href = "login.html";
        }
    } catch (error) {
        console.error('Error loading profile:', error);
        alert('An error occurred while loading the profile.');
    }
}

// Search for a product
async function searchProduct() {
    const productName = document.getElementById("productName").value;
    const resultDiv = document.getElementById("productResult");

    if (!productName) {
        alert("Please enter a product name.");
        return;
    }

    // Clear previous results
    resultDiv.innerHTML = "Searching...";

    try {
        const response = await fetch(API_ENDPOINTS.PRODUCT, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ prod_name: productName })
        });

        const data = await response.json();

        if (response.ok && data.success) {
            const p = data.product;
            const rawCategory = p.category ? p.category.trim() : 'default';
            const rawProvider = p.provider ? p.provider.trim() : 'default';
            const rawName = p.name ? p.name.trim() : 'unknown';
            const imgSrc = `/images/${rawCategory}/${rawProvider}/${rawName}.png`;

            resultDiv.innerHTML = `
                <div class="product-card" style="max-width: 350px; margin: 0 auto;">
                    <div class="product-image-container">
                        <img src="${imgSrc}" alt="${p.name}" class="product-image" onerror="handleImageError(this, '${rawCategory}', '${rawProvider}', '${rawName}')">
                    </div>
                    <div class="product-info">
                        <div class="product-subcategory">${p.subcategory}</div>
                        <h3 class="product-title">${p.name}</h3>
                        <div class="product-meta">
                            <span class="stock-badge">${p.stock} in stock</span>
                            <div class="price-tag">$${p.price}</div>
                        </div>
                        <div class="card-actions">
                            <input type="number" id="qty-${p.id}" value="1" min="1" max="${p.stock}" class="qty-input">
                            <button class="add-btn" onclick="addToCart(${p.id}, '${p.name.replace(/'/g, "\\'")}', ${p.price}, parseInt(document.getElementById('qty-${p.id}').value))">
                                <i class="fas fa-cart-plus"></i> Add to Cart
                        </button>
                    </div>
                </div>
            `;
        } else {
            resultDiv.innerHTML = `<p style="color: red; margin-top: 10px;">${data.message || 'Product not found'}</p>`;
        }
    } catch (error) {
        console.error('Error searching product:', error);
        resultDiv.innerHTML = `<p style="color: red; margin-top: 10px;">An error occurred while searching.</p>`;
    }
}

// Load all products
async function loadProducts() {
    console.log("loadProducts function started");
    const productList = document.getElementById("productList");
    if (!productList) return;
    if (!productList) {
        console.log("productList element not found on this page - stopping.");
        return;
    }

    const urlParams = new URLSearchParams(window.location.search);
    const provider = urlParams.get('provider');

    let url = API_ENDPOINTS.PRODUCT;
    let title = "All Products";
    
    if (provider) {
        url += `?provider=${encodeURIComponent(provider)}`;
        title = `Products from ${provider}`;
    }

    console.log("Fetching products from URL:", url);

    try {
        const response = await fetch(url);
        console.log("Response status:", response.status);
        const data = await response.json();
        console.log("Data received:", data);

        if (response.ok && data.success) {
            let html = `<h2 style="margin-top: 10px; margin-bottom: 20px; color: white; text-shadow: 0 2px 4px rgba(0,0,0,0.2);">${title}</h2>`;
            html += '<div class="products-grid">';

            data.products.forEach(p => {
                // Construct image path based on provider and product name
                // Assuming structure: /data/<ProviderName>/<ProductName>.jpg
                // Added .trim() to remove any invisible CSV characters like \r
                // const safeProvider = p.provider ? encodeURIComponent(p.provider.trim()) : 'default';
                // const safeName = p.name ? encodeURIComponent(p.name.trim()) : 'unknown';
                // const imgSrc = `/images/${safeProvider}/${safeName}.jpg`;
                const rawCategory = p.category ? p.category.trim() : 'default';
                const rawProvider = p.provider ? p.provider.trim() : 'default';
                const rawName = p.name ? p.name.trim() : 'unknown';
                const imgSrc = `/images/${rawCategory}/${rawProvider}/${rawName}.jpg`;
                

            html += `
                <div class="product-card">
                    <div class="product-image-container">
                        <img src="${imgSrc}" 
                            alt="${p.name}" 
                            class="product-image" 
                            onerror="handleImageError(this, '${rawCategory}', '${rawProvider}', '${rawName}')"> 
                    </div>
                    <div class="product-info">
                        <div class="product-subcategory">${p.subcategory}</div>
                        <h3 class="product-title">${p.name}</h3>
                        <div class="product-meta">
                            <span class="stock-badge">${p.stock} in stock</span>
                            <div class="price-tag">$${p.price.toFixed(2)}</div>
                        </div>
                        <div class="card-actions">
                            <input type="number" id="qty-${p.id}" value="1" min="1" max="${p.stock}" class="qty-input">
                            <button class="add-btn" onclick="addToCart(${p.id}, '${p.name.replace(/'/g, "\\'")}', ${p.price}, parseInt(document.getElementById('qty-${p.id}').value))">
                                <i class="fas fa-cart-plus"></i> Add to Cart
                            </button>
                        </div>
                    </div>
                </div>`;
            });
            html += '</div>';
            productList.innerHTML = html;
        } else {
            console.error("Failed to load products. Success flag is false or response not OK.", data);
        }
    } catch (error) {
        console.error('Error loading products:', error);
    }
}

// Image error handler to attempt PNG if JPG fails, then placeholder
function handleImageError(img, category, provider, name) {
    // Prevent infinite loops if the placeholder itself fails
    img.onerror = null;

    // 1. If it failed on JPG, try PNG
    if (img.src.endsWith('.jpg')) {
        console.warn(`JPG failed for ${name}, trying PNG...`);
        img.src = `/images/${category}/${provider}/${name}.png`;
        // Re-attach error handler specifically for the PNG attempt
        img.onerror = function() {
            console.warn(`PNG also failed for ${name}, using placeholder.`);
            this.src = 'https://placehold.co/300x200?text=No+Image';
        };
    } 
    // 2. If it wasn't a JPG (or logic fell through), show placeholder
    else {
        img.src = 'https://placehold.co/300x200?text=No+Image';
    }
}

// ==================Admin page functions ==================
async function loadQueue() {
    const queueDiv = document.getElementById('deliveryQueue');   
    if (!queueDiv) return;

    try {
        const response = await fetch(API_ENDPOINTS.ADMIN_QUEUE);
        const data = await response.json();

        if (data.success) {
            if (data.orders.length === 0) {
                queueDiv.innerHTML = '<p class="empty-msg">No pending orders in the queue.</p>';
                return;
            }

            let html = '<table><thead><tr><th>Order ID</th><th>Customer ID</th><th>Products</th><th>Total</th><th>Status</th></tr></thead><tbody>';
            
            data.orders.forEach(o => {
                html += `<tr>
                    <td><strong>#${o.orderId}</strong></td>
                    <td>${o.customerId}</td>
                    <td>${o.productIds}</td>
                    <td>$${o.totalAmount.toFixed(2)}</td>
                    <td><span class="status-badge">${o.status}</span></td>
                </tr>`;
            });
            html += '</tbody></table>';
            queueDiv.innerHTML = html;
        }
    } catch (error) {
        console.error('Error loading queue:', error);
        queueDiv.innerHTML = '<p style="color:red">Error connecting to server.</p>';
    }
}

async function processNextOrder() {
    try {
        const response = await fetch(API_ENDPOINTS.ADMIN_PROCESS, { method: 'POST' });
        const data = await response.json();
        alert(data.message);
        if (data.success) {
            loadQueue(); // Refresh the table
        }
    } catch (error) {
        console.error('Error processing order:', error);
        alert('Failed to process order.');
    }
}


// ================ CART FUNCTIONS ================

async function addToCart(productId, productName, price, quantity) {
    const customerId = localStorage.getItem("customerId");
    if (!customerId) {
        alert("Please login first!");
        window.location.href = "login.html";
        return;
    }
    
    try {
        const response = await fetch('/api/cart/add', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                customerId: parseInt(customerId),
                productId: productId,
                productName: productName,
                price: price,
                quantity: quantity
            })
        });
        
        const data = await response.json();
        if (data.success) {
            alert(`✓ Added ${quantity} x ${productName} to cart!`);
            updateCartDisplay();
        } else {
            alert("Failed to add to cart: " + data.message);
        }
    } catch (error) {
        console.error('Error adding to cart:', error);
        alert('Failed to add item to cart');
    }
}

async function updateCartDisplay() {
    const customerId = localStorage.getItem("customerId");
    const cartContents = document.getElementById('cartContents');
    const cartDiv = document.getElementById('cartItems');
    const cartCount = document.getElementById('cartCount');
    const cartTotal = document.getElementById('cartTotal');
    const cartBackendCount = document.getElementById('cartBackendCount');
    const emptyState = document.getElementById('emptyState');
    
    if (!customerId) return;
    
    try {
        const response = await fetch(`/api/cart?customerId=${customerId}`);
        const data = await response.json();
        
        if (data.success) {
            // Update cart count badge
            if (cartCount) cartCount.textContent = data.itemCount;
            if (cartBackendCount) cartBackendCount.textContent = data.backendTotalItems;
            
            if (data.itemCount === 0) {
                if (cartContents) cartContents.innerHTML = '<p>Your cart is empty.</p>';
                if (cartTotal) cartTotal.textContent = '$0.00';
                if (cartDiv) cartDiv.style.display = 'none';
                if (emptyState) emptyState.style.display = 'block';
            } else {
                if (emptyState) emptyState.style.display = 'none';
                // Display cart items
                let html = '<div class="cart-items">';
                data.items.forEach(item => {
                    html += `
                        <div class="cart-item">
                            <div>
                                <strong>${item.productName}</strong><br>
                                <span style="color: #666;">${item.quantity} x $${item.price.toFixed(2)}</span>
                            </div>
                            <div style="display: flex; align-items: center; gap: 15px;">
                                <strong style="color: #1976D2; font-size: 1.1rem;">$${item.itemTotal.toFixed(2)}</strong>
                            <button class="remove-btn" onclick="removeFromCart(${item.productId})">
                                <i class="fas fa-trash"></i>
                            </button>
                            </div>
                        </div>
                    `;
                });
                html += '</div>';
                
                if (cartContents) cartContents.innerHTML = html;
                if (cartTotal) cartTotal.textContent = `$${data.total.toFixed(2)}`;
                if (cartDiv) cartDiv.style.display = window.location.pathname.includes('cart.html') ? 'grid' : 'block';
            }
        }
    } catch (error) {
        console.error('Error loading cart:', error);
        if (cartContents) cartContents.innerHTML = '<p style="color: red;">Error loading cart</p>';
    }
}

async function removeFromCart(productId) {
    const customerId = localStorage.getItem("customerId");
    if (!customerId) return;
    
    if (!confirm("Remove this item from cart?")) return;
    
    try {
        const response = await fetch('/api/cart/remove', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                customerId: parseInt(customerId),
                productId: productId
            })
        });
        
        const data = await response.json();
        if (data.success) {
            updateCartDisplay();
        }
    } catch (error) {
        console.error('Error removing from cart:', error);
        alert('Failed to remove item');
    }
}

async function clearCart() {
    const customerId = localStorage.getItem("customerId");
    if (!customerId) return;
    
    if (!confirm("Clear all items from your cart?")) return;
    
    try {
        const response = await fetch('/api/cart/clear', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ customerId: parseInt(customerId) })
        });
        
        const data = await response.json();
        if (data.success) {
            alert(`Cleared ${data.removedCount} items from cart`);
            updateCartDisplay();
        }
    } catch (error) {
        console.error('Error clearing cart:', error);
        alert('Failed to clear cart');
    }
}

async function checkout() {
    const customerId = localStorage.getItem("customerId");
    if (!customerId) return;
    
    try {
        const response = await fetch('/api/cart?customerId=' + customerId);
        const data = await response.json();
        
        if (!data.success || data.itemCount === 0) {
            alert("Your cart is empty!");
            return;
        }
        
        // Show order summary
        let summary = "Order Summary:\n\n";
        data.items.forEach(item => {
            summary += `${item.quantity} x ${item.productName} - $${item.itemTotal.toFixed(2)}\n`;
        });
        summary += `\nTotal: $${data.total.toFixed(2)}\n\n`;
        
        if (confirm(summary + "Proceed with checkout?")) {
            // Get selected payment method
            let paymentMethod = "Cash";
            const cashRadio = document.getElementById("payCash");
            const visaRadio = document.getElementById("payVisa");
            
            if (visaRadio && visaRadio.checked) {
                // Redirect to payment page for Visa
                window.location.href = 'payment.html';
                return;
            }
            
            // Call C++ Checkout API for Cash
            const checkoutResponse = await fetch('/api/cart/checkout', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ 
                    customerId: parseInt(customerId),
                    paymentMethod: "Cash" 
                })
            });
            
            const checkoutData = await checkoutResponse.json();
            
            if (checkoutData.success) {
                alert(`✅ Checkout complete!\n\n` +
                      `Order ID: #${checkoutData.orderId}\n` +
                      `Items purchased: ${checkoutData.itemCount}\n` +
                      `Total: $${checkoutData.total.toFixed(2)}\n\n` +
                      `Thank you for your purchase!`);
                updateCartDisplay();
                // Optionally refresh product list to show updated stock
                if (typeof loadProducts === 'function') loadProducts(); 
            } else {
                alert("Checkout failed: " + checkoutData.message);
            }
        }
    } catch (error) {
        console.error('Error during checkout:', error);
        alert('Checkout failed due to connection error.');
    }
}

// Initialize cart display on product page load
if (window.location.pathname.includes('product.html') || window.location.pathname.includes('cart.html') || window.location.pathname.includes('home.html')) {
    document.addEventListener('DOMContentLoaded', function() {
        updateCartDisplay();
    });
}
// ================ END CART FUNCTIONS ================
