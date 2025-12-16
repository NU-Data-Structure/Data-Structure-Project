// This file now communicates with the C++ backend via API calls.

const API_ENDPOINTS = {
    LOGIN: '/api/login',
    REGISTER: '/api/register',
    PROFILE: '/api/profile',
    PRODUCT: '/api/product'
};

// Login / Sign up function
async function login() {
    const id = document.getElementById("id").value;
    const name = document.getElementById("name").value;
    const phone = document.getElementById("phone").value;
    const address = document.getElementById("address").value;

    if (!id) {
        alert("Customer ID is required.");
        return;
    }

    // If name, phone, and address are provided, it's a registration attempt
    if (name && phone && address) {
        try {
            const response = await fetch(API_ENDPOINTS.REGISTER, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    id: parseInt(id),
                    name: name,
                    phone: phone,
                    address: address
                })
            });

            const data = await response.json();

            if (response.ok && data.success) {
                alert("Registration successful! Redirecting to profile.");
                localStorage.setItem("currentCustomerId", id);
                window.location.href = "profile.html";
            } else {
                alert(`Registration failed: ${data.message || 'Unknown error'}`);
            }
        } catch (error) {
            console.error('Error during registration:', error);
            alert('An error occurred. Please try again.');
        }
    } else {
        // Otherwise, it's a login attempt
        try {
            const response = await fetch(API_ENDPOINTS.LOGIN, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ id: parseInt(id) })
            });

            const data = await response.json();

            if (response.ok && data.exists) {
                localStorage.setItem("currentCustomerId", id);
                window.location.href = "profile.html";
            } else {
                alert("Login failed: Customer ID not found. Please fill in all fields to register.");
            }
        } catch (error) {
            console.error('Error during login:', error);
            alert('An error occurred. Please try again.');
        }
    }
}

// Load profile data
async function loadProfile() {
    const customerId = localStorage.getItem("currentCustomerId");

    if (!customerId) {
        window.location.href = "login.html";
        return;
    }

    try {
        const response = await fetch(`${API_ENDPOINTS.PROFILE}?id=${customerId}`);
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
            resultDiv.innerHTML = `
                <div style="border: 1px solid #ddd; padding: 15px; margin-top: 10px; border-radius: 5px; background-color: #f9f9f9;">
                    <h3>${p.name}</h3>
                    <p><strong>ID:</strong> ${p.id}</p>
                    <p><strong>Price:</strong> $${p.price}</p>
                    <p><strong>Category:</strong> ${p.category} &gt; ${p.subcategory}</p>
                    <p><strong>Stock:</strong> ${p.stock}</p>
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
    const productList = document.getElementById("productList");
    if (!productList) return;

    try {
        const response = await fetch(API_ENDPOINTS.PRODUCT);
        const data = await response.json();

        if (response.ok && data.success) {
            let html = '<h2 style="margin-top: 30px;">All Products</h2>';
            html += '<table style="width:100%; border-collapse: collapse; margin-top: 10px;">';
            html += '<tr style="background-color: #f2f2f2; text-align: left;">';
            html += '<th style="padding: 10px; border: 1px solid #ddd;">ID</th>';
            html += '<th style="padding: 10px; border: 1px solid #ddd;">Name</th>';
            html += '<th style="padding: 10px; border: 1px solid #ddd;">Price</th>';
            html += '<th style="padding: 10px; border: 1px solid #ddd;">Category</th>';
            html += '<th style="padding: 10px; border: 1px solid #ddd;">Subcategory</th>';
            html += '<th style="padding: 10px; border: 1px solid #ddd;">Stock</th>';
            html += '</tr>';

            data.products.forEach(p => {
                html += `<tr>
                    <td style="padding: 10px; border: 1px solid #ddd;">${p.id}</td>
                    <td style="padding: 10px; border: 1px solid #ddd;">${p.name}</td>
                    <td style="padding: 10px; border: 1px solid #ddd;">$${p.price}</td>
                    <td style="padding: 10px; border: 1px solid #ddd;">${p.category}</td>
                    <td style="padding: 10px; border: 1px solid #ddd;">${p.subcategory}</td>
                    <td style="padding: 10px; border: 1px solid #ddd;">${p.stock}</td>
                </tr>`;
            });
            html += '</table>';
            productList.innerHTML = html;
        }
    } catch (error) {
        console.error('Error loading products:', error);
    }
}
