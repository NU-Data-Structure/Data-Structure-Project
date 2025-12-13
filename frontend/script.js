// This file now communicates with the C++ backend via API calls.

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
            const response = await fetch('/api/register', {
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
            const response = await fetch('/api/login', {
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
        const response = await fetch(`/api/profile?id=${customerId}`);
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
