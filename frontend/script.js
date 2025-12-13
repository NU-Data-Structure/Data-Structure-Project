// This array simulates your Customer SLL
let customers = JSON.parse(localStorage.getItem("customers")) || [];

// Login / Sign up function
function login() {
    const id = document.getElementById("id").value;
    const name = document.getElementById("name").value;
    const phone = document.getElementById("phone").value;
    const address = document.getElementById("address").value;

    if (!id) {
        alert("Customer ID is required");
        return;
    }

    // Search customer (like searchID in SLL)
    let customer = customers.find(c => c.id == id);

    if (customer) {
        // Existing customer
        localStorage.setItem("currentCustomer", JSON.stringify(customer));
        window.location.href = "profile.html";
    } else {
        // New customer
        if (!name || !phone || !address) {
            alert("Please enter all details for new account");
            return;
        }

        let newCustomer = {
            id,
            name,
            phone,
            address
        };

        customers.push(newCustomer); // addCustomer (SLL equivalent)

        localStorage.setItem("customers", JSON.stringify(customers));
        localStorage.setItem("currentCustomer", JSON.stringify(newCustomer));

        window.location.href = "profile.html";
    }
}

// Load profile data
function loadProfile() {
    let customer = JSON.parse(localStorage.getItem("currentCustomer"));

    if (!customer) {
        window.location.href = "login.html";
        return;
    }

    document.getElementById("profile").innerHTML = `
        <b>ID:</b> ${customer.id}<br>
        <b>Name:</b> ${customer.name}<br>
        <b>Phone:</b> ${customer.phone}<br>
        <b>Address:</b> ${customer.address}
    `;
}
