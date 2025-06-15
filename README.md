📌 "Employee Attendance Tracker "(C Language Project)"

This is a simple "console-based Employee Attendance Tracker" built using "C". It allows both admins and employees to interact with the system to manage employee records and mark daily attendance. The data is stored in CSV files, making it easy to access and portable.


🧾 "Features"

👨‍💼 Admin Dashboard

🟢 Add new employees

🟢 View employee list

🟢 Update employee details

🟢 Delete employees

🟢 Auto-save changes to "employees.csv"





👤 User Dashboard

🟢 Mark attendance (one-time per day)

🟢 View today's attendance records




📁 File-Based Storage

🗃️ Employee details: "employees.csv"

🗃️ Attendance records: "attendance_YYYY-MM-DD.csv" (auto-generated daily)



🔧 "How It Works"

🔹 On startup, the program reads "employees.csv" and loads employee data into a linked list.

🔹 Users can log in as:

  🔑 Admin with hardcoded credentials:
  
   👤 Username: "admin"
   
   🔒 Password: "password"
   
  👤 User using just the employee ID to mark attendance.
  
🔹 Attendance is saved with date and status ""Present"".




📂 "File Structure"


"
📁 Project Root

├── main.c                     → Main source code

├── employees.csv              → Stores employee ID and name

├── attendance_YYYY-MM-DD.csv → Auto-generated daily for attendance

"



▶️ "How to Run"

1️⃣ Make sure you have a C compiler (like "gcc") installed.

2️⃣ Compile the code:

"bash

gcc main.c -o attendance

"

3️⃣ Run the executable:


"bash

./attendance

"


📝 "Sample CSV Format"

📄 employees.csv

"
1,John Doe

2,Jane Smith

"


📄 attendance\_2025-06-15.csv

"

1,John Doe,2025-06-15,Present

2,Jane Smith,2025-06-15,Present

"



✅ "Future Improvements"

🟨 Add password protection for each employee

🟨 GUI version using a C-based toolkit

🟨 Export attendance as PDF

🟨 Admin can view past attendance by date


📜 "License"


This project is open-source and free to use under the MIT License.


