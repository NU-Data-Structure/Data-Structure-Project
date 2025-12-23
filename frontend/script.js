// This file now communicates with the C++ backend via API calls.
console.log("script.js loaded successfully");

const API_ENDPOINTS = {
    LOGIN: '/api/login',
    REGISTER: '/api/register',
    PROFILE: '/api/profile',
    PRODUCT: '/api/product',
    ADMIN_QUEUE: '/api/admin/deliveryQueue',
    ADMIN_PROCESS: '/api/admin/processOrder',
    PROVIDERS: '/api/providers',
    ADMIN_ADD_PROD: '/api/admin/addProduct',
    ADMIN_DEL_PROD: '/api/admin/deleteProduct'
};

// ================== Theme Toggle Logic ==================
function initThemeToggle() {
    // 1. Inject Enhanced Blue Gradient CSS for Both Themes
    const style = document.createElement('style');
    style.innerHTML = `
        /* ============================================
           ENHANCED BLUE GRADIENT THEME SYSTEM
           Only Blue & Light Blue Colors
           ============================================ */

        /* Toggle Button Style - Blue Gradient */
        .theme-toggle-btn {
            position: fixed;
            bottom: 25px;
            left: 25px;
            width: 55px;
            height: 55px;
            border-radius: 50%;
            background: linear-gradient(135deg, #1e40af 0%, #3b82f6 50%, #0ea5e9 100%);
            backdrop-filter: blur(15px);
            border: 2px solid rgba(56, 189, 248, 0.4);
            color: white;
            font-size: 1.5rem;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            z-index: 1000;
            transition: all 0.4s cubic-bezier(0.4, 0, 0.2, 1);
            box-shadow: 0 8px 32px rgba(59, 130, 246, 0.4),
                        0 0 20px rgba(14, 165, 233, 0.3);
        }

        .theme-toggle-btn:hover {
            transform: scale(1.15) rotate(20deg);
            box-shadow: 0 12px 40px rgba(59, 130, 246, 0.5),
                        0 0 30px rgba(14, 165, 233, 0.4);
        }

        body.light-theme .theme-toggle-btn {
            background: linear-gradient(135deg, #0284c7 0%, #0ea5e9 50%, #38bdf8 100%) !important;
            color: white !important;
            border: 2px solid rgba(56, 189, 248, 0.5) !important;
            box-shadow: 0 8px 32px rgba(14, 165, 233, 0.35),
                        0 0 20px rgba(56, 189, 248, 0.25) !important;
        }

        /* ============================================
           ENHANCED LIGHT THEME - Blue Gradients
           ============================================ */
        
        body.light-theme {
            background: linear-gradient(135deg, 
                #f0f9ff 0%, 
                #e0f2fe 25%, 
                #bae6fd 50%, 
                #7dd3fc 75%,
                #38bdf8 100%) !important;
            background-attachment: fixed !important;
            color: #0c4a6e !important;
        }
        
        body.light-theme::before {
            content: '';
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background: 
                radial-gradient(ellipse at 20% 20%, rgba(14, 165, 233, 0.15) 0%, transparent 50%),
                radial-gradient(ellipse at 80% 80%, rgba(56, 189, 248, 0.12) 0%, transparent 50%),
                radial-gradient(ellipse at 50% 50%, rgba(125, 211, 252, 0.1) 0%, transparent 60%);
            pointer-events: none;
            z-index: 0;
        }
        
        body.light-theme::after {
            content: '';
            position: fixed;
            top: -50%;
            left: -50%;
            right: -50%;
            bottom: -50%;
            background: linear-gradient(45deg,
                transparent 40%,
                rgba(56, 189, 248, 0.04) 45%,
                rgba(14, 165, 233, 0.06) 50%,
                rgba(56, 189, 248, 0.04) 55%,
                transparent 60%);
            pointer-events: none;
            z-index: 0;
            animation: shimmerLight 8s ease-in-out infinite;
        }

        @keyframes shimmerLight {
            0%, 100% { transform: translateX(-20%) translateY(-20%) rotate(0deg); }
            50% { transform: translateX(20%) translateY(20%) rotate(5deg); }
        }

        /* Containers - Light Theme Blue Glass */
        body.light-theme .container,
        body.light-theme .login-container,
        body.light-theme .payment-container,
        body.light-theme .orders-section,
        body.light-theme .cart-section,
        body.light-theme .profile-card,
        body.light-theme .provider-card {
            background: linear-gradient(135deg, 
                rgba(255, 255, 255, 0.9) 0%, 
                rgba(224, 242, 254, 0.85) 50%,
                rgba(186, 230, 253, 0.8) 100%) !important;
            backdrop-filter: blur(25px) !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
            box-shadow: 0 25px 60px rgba(14, 165, 233, 0.15),
                        0 0 40px rgba(56, 189, 248, 0.1),
                        inset 0 1px 0 rgba(255, 255, 255, 0.8) !important;
            color: #0c4a6e !important;
        }

        /* Product Cards - Light Theme */
        body.light-theme .product-card {
            background: linear-gradient(145deg, 
                rgba(255, 255, 255, 0.95) 0%, 
                rgba(240, 249, 255, 0.9) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.25) !important;
            box-shadow: 0 8px 32px rgba(14, 165, 233, 0.12),
                        0 4px 16px rgba(56, 189, 248, 0.08) !important;
            transition: all 0.4s cubic-bezier(0.4, 0, 0.2, 1) !important;
        }

        body.light-theme .product-card:hover {
            transform: translateY(-8px) scale(1.02) !important;
            border-color: rgba(14, 165, 233, 0.5) !important;
            box-shadow: 0 20px 50px rgba(14, 165, 233, 0.2),
                        0 8px 24px rgba(56, 189, 248, 0.15),
                        0 0 20px rgba(125, 211, 252, 0.1) !important;
        }

        /* Headers - Light Theme Blue Gradient */
        body.light-theme header, 
        body.light-theme .header {
            background: linear-gradient(135deg, 
                rgba(255, 255, 255, 0.92) 0%, 
                rgba(224, 242, 254, 0.88) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
            box-shadow: 0 8px 32px rgba(14, 165, 233, 0.12) !important;
        }

        body.light-theme .nav-buttons button,
        body.light-theme .back-btn {
            background: linear-gradient(135deg, 
                rgba(255, 255, 255, 0.95) 0%, 
                rgba(224, 242, 254, 0.9) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
            color: #0369a1 !important;
            box-shadow: 0 4px 15px rgba(14, 165, 233, 0.1) !important;
            transition: all 0.3s ease !important;
        }

        body.light-theme .nav-buttons button:hover,
        body.light-theme .back-btn:hover {
            background: linear-gradient(135deg, #0ea5e9, #38bdf8) !important;
            color: white !important;
            border-color: transparent !important;
            box-shadow: 0 8px 25px rgba(14, 165, 233, 0.3) !important;
            transform: translateY(-2px) !important;
        }

        /* Headings - Light Theme Blue Gradient Text */
        body.light-theme h1, 
        body.light-theme h2, 
        body.light-theme h3, 
        body.light-theme .profile-header h1,
        body.light-theme .product-title,
        body.light-theme .provider-name {
            background: linear-gradient(135deg, #0c4a6e 0%, #0369a1 50%, #0284c7 100%) !important;
            -webkit-background-clip: text !important;
            -webkit-text-fill-color: transparent !important;
            background-clip: text !important;
            text-shadow: none !important;
        }

        /* Stock Badge - Light Theme Blue */
        body.light-theme .stock-badge {
            background: linear-gradient(135deg, rgba(14, 165, 233, 0.15), rgba(56, 189, 248, 0.2)) !important;
            color: #0369a1 !important;
            border: 1px solid rgba(14, 165, 233, 0.3) !important;
        }

        /* Price Tag - Light Theme */
        body.light-theme .price-tag {
            background: linear-gradient(135deg, #0284c7, #0ea5e9, #38bdf8) !important;
            -webkit-background-clip: text !important;
            -webkit-text-fill-color: transparent !important;
        }

        /* Inputs - Light Theme Blue Focus */
        body.light-theme input,
        body.light-theme .qty-input {
            background: linear-gradient(135deg, #ffffff, #f0f9ff) !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
            color: #0c4a6e !important;
            transition: all 0.3s ease !important;
        }

        body.light-theme input:focus,
        body.light-theme .qty-input:focus {
            background: white !important;
            border-color: #0ea5e9 !important;
            box-shadow: 0 0 0 4px rgba(14, 165, 233, 0.15),
                        0 4px 12px rgba(56, 189, 248, 0.1) !important;
        }

        /* Buttons - Light Theme Blue Gradient */
        body.light-theme .btn,
        body.light-theme .search-btn,
        body.light-theme .add-btn {
            background: linear-gradient(135deg, #0284c7 0%, #0ea5e9 50%, #38bdf8 100%) !important;
            color: white !important;
            border: none !important;
            box-shadow: 0 6px 20px rgba(14, 165, 233, 0.35) !important;
        }

        body.light-theme .btn:hover,
        body.light-theme .search-btn:hover,
        body.light-theme .add-btn:hover {
            background: linear-gradient(135deg, #0369a1 0%, #0284c7 50%, #0ea5e9 100%) !important;
            box-shadow: 0 10px 30px rgba(14, 165, 233, 0.45) !important;
            transform: translateY(-3px) !important;
        }

        /* View Cart Button - Light Theme */
        body.light-theme .view-cart-btn {
            background: linear-gradient(135deg, #0369a1 0%, #0284c7 50%, #0ea5e9 100%) !important;
            color: white !important;
            box-shadow: 0 6px 20px rgba(14, 165, 233, 0.35) !important;
        }

        /* Nav Buttons */
        body.light-theme .nav-btn,
        body.light-theme .action-buttons .btn-secondary {
            background: linear-gradient(135deg, #ffffff, #f0f9ff) !important;
            color: #0369a1 !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
            box-shadow: 0 4px 12px rgba(14, 165, 233, 0.1) !important;
        }

        body.light-theme .nav-btn:hover {
            background: linear-gradient(135deg, #0ea5e9, #38bdf8) !important;
            color: white !important;
            border-color: transparent !important;
        }

        /* Search Box - Light Theme */
        body.light-theme .search-box {
            background: linear-gradient(135deg, 
                rgba(255, 255, 255, 0.9) 0%, 
                rgba(224, 242, 254, 0.85) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
            box-shadow: 0 8px 32px rgba(14, 165, 233, 0.1) !important;
        }

        /* Admin Dashboard Light Theme */
        body.light-theme .sidebar {
            background: linear-gradient(180deg, 
                rgba(255, 255, 255, 0.95) 0%, 
                rgba(224, 242, 254, 0.9) 100%) !important;
            border-right: 1px solid rgba(56, 189, 248, 0.25) !important;
        }
        
        body.light-theme .main-content {
            background: linear-gradient(135deg, #f0f9ff, #e0f2fe) !important;
        }

        body.light-theme .add-form {
            background: linear-gradient(135deg, 
                rgba(255, 255, 255, 0.95) 0%, 
                rgba(240, 249, 255, 0.9) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.25) !important;
        }

        /* Tables - Light Theme Blue */
        body.light-theme table {
            background: linear-gradient(135deg, #ffffff, #f0f9ff) !important;
            border: 1px solid rgba(56, 189, 248, 0.25) !important;
            color: #0c4a6e !important;
            border-radius: 16px !important;
            overflow: hidden;
        }

        body.light-theme th {
            background: linear-gradient(135deg, #e0f2fe, #bae6fd) !important;
            color: #0369a1 !important;
            border-bottom: 1px solid rgba(56, 189, 248, 0.3) !important;
        }

        body.light-theme td {
            border-bottom: 1px solid rgba(56, 189, 248, 0.15) !important;
            color: #0c4a6e !important;
        }

        body.light-theme tr:hover {
            background: linear-gradient(135deg, 
                rgba(224, 242, 254, 0.5), 
                rgba(186, 230, 253, 0.4)) !important;
        }

        /* Nav Items - Light Theme */
        body.light-theme .nav-item {
            color: #0369a1 !important;
            transition: all 0.3s ease !important;
        }

        body.light-theme .nav-item:hover,
        body.light-theme .nav-item.active {
            background: linear-gradient(135deg, #0284c7, #0ea5e9) !important;
            color: white !important;
        }
        
        body.light-theme h2 {
            border-bottom-color: rgba(56, 189, 248, 0.3) !important;
        }

        /* User Type Toggle - Light Theme */
        body.light-theme .user-type-toggle {
            background: linear-gradient(135deg, 
                rgba(224, 242, 254, 0.8), 
                rgba(186, 230, 253, 0.7)) !important;
            border: 1px solid rgba(56, 189, 248, 0.3) !important;
        }

        body.light-theme .slider {
            background: linear-gradient(135deg, #e0f2fe, #bae6fd) !important;
        }

        body.light-theme .slider:before {
            background: linear-gradient(135deg, #0284c7, #0ea5e9) !important;
        }

        body.light-theme .toggle-label.active {
            color: #0284c7 !important;
        }

        /* Welcome Message - Light Theme */
        body.light-theme .welcome-message p {
            color: #0c4a6e !important;
        }

        body.light-theme .welcome-message .subtext {
            color: #0369a1 !important;
        }

        /* Footer Text - Light Theme */
        body.light-theme .footer-text {
            color: #0369a1 !important;
        }

        body.light-theme .footer-text a {
            background: linear-gradient(135deg, #0284c7, #0ea5e9, #38bdf8) !important;
            -webkit-background-clip: text !important;
            -webkit-text-fill-color: transparent !important;
        }

        /* Category Cards - Light Theme */
        body.light-theme .category-card {
            background: linear-gradient(145deg, 
                rgba(255, 255, 255, 0.95) 0%, 
                rgba(240, 249, 255, 0.9) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.25) !important;
            box-shadow: 0 8px 32px rgba(14, 165, 233, 0.12) !important;
        }

        body.light-theme .category-card:hover {
            border-color: rgba(14, 165, 233, 0.5) !important;
            box-shadow: 0 15px 40px rgba(14, 165, 233, 0.2) !important;
        }

        /* Cart Items - Light Theme */
        body.light-theme .cart-item {
            background: linear-gradient(135deg, #ffffff, #f0f9ff) !important;
            border: 1px solid rgba(56, 189, 248, 0.2) !important;
        }

        body.light-theme .remove-btn {
            background: linear-gradient(135deg, #0284c7, #0ea5e9) !important;
            color: white !important;
        }

        /* Order Cards - Light Theme */
        body.light-theme .order-card {
            background: linear-gradient(145deg, 
                rgba(255, 255, 255, 0.95) 0%, 
                rgba(240, 249, 255, 0.9) 100%) !important;
            border: 1px solid rgba(56, 189, 248, 0.25) !important;
        }

        body.light-theme .status-badge {
            background: linear-gradient(135deg, rgba(14, 165, 233, 0.15), rgba(56, 189, 248, 0.2)) !important;
            color: #0369a1 !important;
            border: 1px solid rgba(14, 165, 233, 0.3) !important;
        }

        /* ============================================
           ENHANCED DARK THEME - Deepest Navy & Gradients
           ============================================ */
        
        body.dark-theme {
            background: linear-gradient(135deg, 
                #020617 0%, 
                #0b1120 25%, 
                #0f172a 50%, 
                #172554 80%,
                #1e3a8a 100%) !important;
            background-attachment: fixed !important;
            color: #e2e8f0 !important;
        }
        
        body.dark-theme::before {
            content: '';
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background: 
                radial-gradient(circle at 15% 50%, rgba(29, 78, 216, 0.15) 0%, transparent 25%),
                radial-gradient(circle at 85% 30%, rgba(14, 165, 233, 0.1) 0%, transparent 25%);
            pointer-events: none;
            z-index: 0;
        }

        /* Dark Theme Containers - Glassmorphism */
        body.dark-theme .container,
        body.dark-theme .login-container,
        body.dark-theme .payment-container,
        body.dark-theme .orders-section,
        body.dark-theme .cart-section,
        body.dark-theme .profile-card,
        body.dark-theme .provider-card,
        body.dark-theme .sidebar,
        body.dark-theme .add-form,
        body.dark-theme .search-box,
        body.dark-theme .header,
        body.dark-theme header {
            background: linear-gradient(145deg, 
                rgba(15, 23, 42, 0.9) 0%, 
                rgba(30, 41, 59, 0.85) 100%) !important;
            backdrop-filter: blur(20px) !important;
            border: 1px solid rgba(30, 58, 138, 0.3) !important;
            box-shadow: 0 25px 50px rgba(0, 0, 0, 0.5),
                        inset 0 1px 0 rgba(255, 255, 255, 0.05) !important;
            color: #f1f5f9 !important;
        }

        /* Dark Theme Product Cards */
        body.dark-theme .product-card,
        body.dark-theme .category-card,
        body.dark-theme .order-card {
            background: linear-gradient(145deg, 
                rgba(30, 41, 59, 0.8) 0%, 
                rgba(15, 23, 42, 0.9) 100%) !important;
            border: 1px solid rgba(59, 130, 246, 0.15) !important;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.4) !important;
        }

        body.dark-theme .product-card:hover,
        body.dark-theme .category-card:hover {
            transform: translateY(-8px) scale(1.02) !important;
            border-color: rgba(96, 165, 250, 0.4) !important;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.6),
                        0 0 20px rgba(59, 130, 246, 0.2) !important;
        }

        /* Dark Theme Text Gradients */
        body.dark-theme h1, 
        body.dark-theme h2, 
        body.dark-theme h3,
        body.dark-theme .active-link {
            background: linear-gradient(135deg, #60a5fa 0%, #3b82f6 50%, #2563eb 100%) !important;
            -webkit-background-clip: text !important;
            -webkit-text-fill-color: transparent !important;
            text-shadow: 0 0 30px rgba(59, 130, 246, 0.3) !important;
        }

        /* Dark Theme Inputs */
        body.dark-theme input,
        body.dark-theme .qty-input {
            background: rgba(15, 23, 42, 0.6) !important;
            border: 1px solid rgba(59, 130, 246, 0.2) !important;
            color: #f1f5f9 !important;
        }

        body.dark-theme input:focus {
            background: rgba(15, 23, 42, 0.9) !important;
            border-color: #3b82f6 !important;
            box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.25) !important;
        }

        /* Dark Theme Buttons */
        body.dark-theme .btn,
        body.dark-theme button,
        body.dark-theme .nav-icon {
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3) !important;
        }

        body.dark-theme .nav-item {
            color: #94a3b8 !important;
        }

        body.dark-theme .nav-item:hover,
        body.dark-theme .nav-item.active {
            background: linear-gradient(135deg, rgba(59, 130, 246, 0.2), rgba(37, 99, 235, 0.2)) !important;
            color: #60a5fa !important;
            box-shadow: 0 0 20px rgba(59, 130, 246, 0.15) !important;
        }

        body.dark-theme table {
            background: rgba(15, 23, 42, 0.6) !important;
            color: #cbd5e1 !important;
        }

        body.dark-theme th {
            background: rgba(30, 41, 59, 0.8) !important;
            color: #93c5fd !important;
            border-bottom: 1px solid rgba(59, 130, 246, 0.2) !important;
        }

        body.dark-theme td {
            border-bottom: 1px solid rgba(30, 41, 59, 0.5) !important;
        }
        
        body.dark-theme tr:hover {
            background: rgba(30, 41, 59, 0.5) !important;
        }
        
        /* Smooth transitions */
        body, body * {
            transition: background 0.4s ease, 
                        background-color 0.4s ease, 
                        border-color 0.4s ease,
                        box-shadow 0.4s ease,
                        color 0.3s ease !important;
        }
    `;
    document.head.appendChild(style);

    // 2. Inject Toggle Button with Blue Gradient
    const btn = document.createElement('button');
    btn.className = 'theme-toggle-btn';
    btn.innerHTML = '<i class="fas fa-sun"></i>'; // Default to Sun (Light Mode)
    btn.onclick = toggleTheme;
    btn.title = "Toggle Light/Dark Theme";
    document.body.appendChild(btn);

    // 3. Load Saved Theme
    const currentTheme = localStorage.getItem('theme');
    if (currentTheme === 'dark') {
        document.body.classList.add('dark-theme');
        btn.innerHTML = '<i class="fas fa-moon"></i>';
        // Dark theme active
    } else {
        document.body.classList.remove('dark-theme'); // Ensure clean state
        btn.innerHTML = '<i class="fas fa-sun"></i>';
    }
}

function toggleTheme() {
    const body = document.body;
    const btn = document.querySelector('.theme-toggle-btn');

    // Toggle dark-theme class
    body.classList.toggle('dark-theme');

    if (body.classList.contains('dark-theme')) {
        localStorage.setItem('theme', 'dark');
        btn.innerHTML = '<i class="fas fa-moon"></i>';

        // Add specific dark mode styles dynamically
        btn.style.background = 'linear-gradient(135deg, #1e40af 0%, #3b82f6 50%, #0ea5e9 100%)';
        btn.style.color = 'white';
        btn.style.boxShadow = '0 0 20px rgba(59, 130, 246, 0.5)';
    } else {
        localStorage.setItem('theme', 'light');
        btn.innerHTML = '<i class="fas fa-sun"></i>';

        // Return to default light mode styles
        btn.style.background = '';
        btn.style.color = '';
        btn.style.boxShadow = '';
    }
}
// ========================================================

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
            alert(`Registration failed: ${data.message || 'Unknown error'} `);
        }
    } catch (error) {
        console.error('Error during registration:', error);
        alert('An error occurred. Please try again.');
    }
}

// Load profile data
async function loadProfile() {
    const customerId = localStorage.getItem("customerId");
    const password = localStorage.getItem("customerPassword");

    if (!customerId) {
        window.location.href = "login.html";
        return;
    }

    try {
        const response = await fetch(`${API_ENDPOINTS.PROFILE}?id = ${customerId}& password=${password} `);
        if (response.ok) {
            const customer = await response.json();
            document.getElementById("profile").innerHTML = `
        < b > ID:</b > ${customer.id} <br>
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

// Sort products by price (Low to High) using counting sort on backend
async function sortProductsByPrice() {
    console.log("sortProductsByPrice function started");
    const productList = document.getElementById("productList");
    if (!productList) return;

    const urlParams = new URLSearchParams(window.location.search);
    const provider = urlParams.get('provider');

    let url = '/api/product/sorted';
    let title = "All Products (Sorted by Price: Low → High)";

    if (provider) {
        url += `?provider=${encodeURIComponent(provider)}`;
        title = `Products from ${provider} (Sorted by Price: Low → High)`;
    }

    console.log("Fetching sorted products from URL:", url);

    try {
        const response = await fetch(url);
        console.log("Response status:", response.status);
        const data = await response.json();
        console.log("Sorted data received:", data);

        if (response.ok && data.success) {
            let html = `<h2 style="margin-top: 10px; margin-bottom: 20px; color: white; text-shadow: 0 2px 4px rgba(0,0,0,0.2);">
                <i class="fas fa-sort-amount-up-alt" style="color: #10b981;"></i> ${title}
            </h2>`;
            html += '<div class="products-grid">';

            data.products.forEach(p => {
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

            // Show a notification
            console.log(`Sorted ${data.products.length} products by price (low to high)`);
        } else {
            console.error("Failed to load sorted products. Success flag is false or response not OK.", data);
        }
    } catch (error) {
        console.error('Error loading sorted products:', error);
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
        img.onerror = function () {
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

    const provider = localStorage.getItem('adminName');

    try {
        const url = provider
            ? `${API_ENDPOINTS.ADMIN_QUEUE}?provider=${encodeURIComponent(provider)}`
            : API_ENDPOINTS.ADMIN_QUEUE;
        const response = await fetch(url);
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
    const provider = localStorage.getItem('adminName');
    if (!provider) {
        alert("Please login as provider first.");
        return;
    }

    try {
        const response = await fetch(API_ENDPOINTS.ADMIN_PROCESS, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ provider: provider })
        });
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

// ================== Admin Dashboard Logic ==================

function loadAdminDashboard() {
    const providerName = localStorage.getItem('adminName');
    const providerImage = localStorage.getItem('adminImage');

    if (!providerName) {
        window.location.href = 'login.html';
        return;
    }

    document.getElementById('adminName').textContent = providerName;

    const imgEl = document.getElementById('adminImg');
    if (providerImage && providerImage !== "") {
        imgEl.src = providerImage;
    } else {
        // Fallback image
        imgEl.src = `https://ui-avatars.com/api/?name=${encodeURIComponent(providerName)}&background=random`;
    }

    loadAdminProducts();
    loadQueue();
}

function switchAdminTab(tabId, navElement) {
    // Hide all sections
    document.querySelectorAll('.section').forEach(el => el.classList.remove('active'));
    // Show selected section
    document.getElementById(tabId).classList.add('active');

    // Update nav active state
    document.querySelectorAll('.nav-item').forEach(el => el.classList.remove('active'));
    if (navElement) navElement.classList.add('active');
}

async function loadAdminProducts() {
    const tbody = document.getElementById('adminProductList');
    const provider = localStorage.getItem('adminName');
    if (!tbody || !provider) return;

    try {
        const response = await fetch(`${API_ENDPOINTS.PRODUCT}?provider=${encodeURIComponent(provider)}`);
        const data = await response.json();

        if (data.success) {
            let html = '';
            data.products.forEach(p => {
                html += `
                    <tr>
                        <td>${p.id}</td>
                        <td>${p.name}</td>
                        <td>$${p.price}</td>
                        <td>${p.stock}</td>
                        <td>
                            <button class="btn btn-danger" onclick="adminDeleteProduct(${p.id})">Delete</button>
                        </td>
                    </tr>
                `;
            });
            tbody.innerHTML = html;
        }
    } catch (e) {
        console.error(e);
    }
}

async function adminAddProduct() {
    const id = document.getElementById('newId').value;
    const name = document.getElementById('newName').value;
    const price = document.getElementById('newPrice').value;
    const stock = document.getElementById('newStock').value;
    const subCat = document.getElementById('newSubCat').value;
    const provider = localStorage.getItem('adminName');

    if (!id || !name || !price || !stock) {
        alert("Please fill required fields");
        return;
    }
    loadAdminProducts();

    const response = await fetch(API_ENDPOINTS.ADMIN_ADD_PROD, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            id: parseInt(id), name, price: parseFloat(price),
            stock: parseInt(stock), provider, subcategory: subCat
        })
    });

    const data = await response.json();
    if (data.success) {
        loadAdminProducts();
        // Clear inputs
        document.querySelectorAll('.add-form input').forEach(i => i.value = '');
    } else {
        alert(data.message || "Failed to add product");
    }
}

async function adminDeleteProduct(id) {
    if (!confirm("Delete this product?")) return;

    const response = await fetch(API_ENDPOINTS.ADMIN_DEL_PROD, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id })
    });

    if ((await response.json()).success) {
        loadAdminProducts();
    } else {
        alert("Failed to delete product");
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

// Initialize theme and other components
document.addEventListener('DOMContentLoaded', function () {
    // initThemeToggle instantiates the CSS and Button.
    if (typeof initThemeToggle === 'function') initThemeToggle();

    if (window.location.pathname.includes('product.html') || window.location.pathname.includes('cart.html') || window.location.pathname.includes('home.html')) {
        if (typeof updateCartDisplay === 'function') updateCartDisplay();
    }
});
