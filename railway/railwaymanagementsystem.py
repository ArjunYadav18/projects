import mysql.connector as sc

# Database Connection
def db_connect():
    return sc.connect(host="localhost", user="root", password="mgm", database="railway_reservation")

# Home Page
def home():
    print("\nWELCOME TO INDIAN RAILWAY")
    options = {
        1: add_train,
        2: update_train,   # Ensure update_train function exists
        3: remove_train,   # Ensure remove_train function exists
        4: search_train,   # Ensure search_train function exists
        5: book_ticket,    # Ensure book_ticket function exists
        6: cancel_ticket,  # Ensure cancel_ticket function exists
        7: search_booking, # Ensure search_booking function exists
        8: report          # Ensure report function exists
    }
    for key, value in options.items():
        print(f"{key}. {value.__name__.replace('_', ' ').title()}")

    choice = int(input("Enter your choice: "))
    selected_function = options.get(choice)
    if selected_function:
        selected_function()  # Call the selected function
    else:
        print("Invalid Option")

# Login Function
def login():
    username = input("Enter Username: ")
    password = input("Enter Password: ")

    if username == 'Admin' and password == 'mgm':
        print("Login Successful!")
        home()
    else:
        print("Invalid Credentials")
        login()

# Add Train Function
def add_train():
    details = {
        "ID": input("Train ID: "),
        "Name": input("Train Name: "),
        "Start": input("Starting Location: "),
        "End": input("Ending Location: "),
        "StartTime": input("Starting Time (HH:MM:SS): "),
        "EndTime": input("Ending Time (HH:MM:SS): "),
        "AC": int(input("Number of AC coaches: ")),
        "Sleeper": int(input("Number of Sleeper coaches: ")),
        "General": int(input("Number of General coaches: ")),
        "PriceAC": int(input("AC Ticket Price: ")),
        "PriceSleeper": int(input("Sleeper Ticket Price: ")),
        "PriceGeneral": int(input("General Ticket Price: "))
    }

    conn = db_connect()
    cursor = conn.cursor()
    query = "INSERT INTO train VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
    cursor.execute(query, tuple(details.values()))
    conn.commit()
    print("Train Added Successfully!")
    conn.close()

def update_train():
    train_id = input("Enter Train ID to update: ")
    updates = {
        "StartTime": input("New Start Time (HH:MM:SS): "),
        "EndTime": input("New End Time (HH:MM:SS): "),
        "AC": int(input("New AC Count: ")),
        "Sleeper": int(input("New Sleeper Count: ")),
        "General": int(input("New General Count: ")),
        "PriceAC": int(input("New AC Ticket Price: ")),
        "PriceSleeper": int(input("New Sleeper Ticket Price: ")),
        "PriceGeneral": int(input("New General Ticket Price: "))
    }

    conn = db_connect()
    cursor = conn.cursor()
    query = """UPDATE train SET starttime=%s, endtime=%s, NoofAC=%s, noofsleeper=%s, 
               noofgeneral=%s, priceAc=%s, pricesleeper=%s, pricegeneral=%s WHERE Tid=%s"""
    cursor.execute(query, (*updates.values(), train_id))
    conn.commit()
    print("Train Updated Successfully!")
    conn.close()

# Search Train Function
def search_train():
    train_id = input("Enter Train ID: ")

    conn = db_connect()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM train WHERE Tid=%s", (train_id,))
    record = cursor.fetchone()

    if record:
        print("Train Details:", record)
    else:
        print("No train found with this ID")
    
    conn.close()

# Book Ticket Function
def book_ticket():
    conn = db_connect()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM train")
    trains = cursor.fetchall()

    if not trains:
        print("No Trains Available")
        return

    print("Available Trains:")
    for train in trains:
        print(train)

    train_id = input("Enter Train ID: ")
    ticket_type = input("Ticket Type (AC/Sleeper/General): ").capitalize()
    name = input("Your Name: ")
    age = int(input("Your Age: "))
    aadhar = input("Aadhar Number: ")

    cursor.execute("SELECT * FROM train WHERE Tid=%s", (train_id,))
    train = cursor.fetchone()

    if not train:
        print("Invalid Train ID")
        return

    fare = {"AC": train[9], "Sleeper": train[10], "General": train[11]}[ticket_type]
    seat_count = {"AC": 6, "Sleeper": 7, "General": 8}[ticket_type]

    if train[seat_count] <= 0:
        print("No Seats Available")
        return

    if age >= 60 or age <= 10:
        fare *= 0.5  # 50% Discount

    cursor.execute(f"UPDATE train SET NoOf{ticket_type} = NoOf{ticket_type} - 1 WHERE Tid=%s", (train_id,))
    cursor.execute("INSERT INTO train_booking (Tid, TicketType, PassengerName, Age, AadharNo, Fare) VALUES (%s, %s, %s, %s, %s, %s)", 
                   (train_id, ticket_type, name, age, aadhar, fare))
    
    conn.commit()
    print("Ticket Booked Successfully!")
    conn.close()

# Cancel Ticket Function
def cancel_ticket():
    aadhar = input("Enter Aadhar Number: ")

    conn = db_connect()
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM train_booking WHERE AadharNo=%s", (aadhar,))
    booking = cursor.fetchone()

    if not booking:
        print("No Booking Found")
        return

    cursor.execute("DELETE FROM train_booking WHERE AadharNo=%s", (aadhar,))
    seat_column = {"AC": "NoOfAC", "Sleeper": "NoOfSleeper", "General": "NoOfGeneral"}[booking[2]]
    cursor.execute(f"UPDATE train SET {seat_column} = {seat_column} + 1 WHERE Tid=%s", (booking[1],))
    
    conn.commit()
    print("Ticket Cancelled Successfully!")
    conn.close()

# Report Function
def report():
    conn = db_connect()
    cursor = conn.cursor()
    cursor.execute("SELECT TicketType, COUNT(*) FROM train_booking GROUP BY TicketType")
    records = cursor.fetchall()

    if records:
        print("Ticket Booking Report:")
        for record in records:
            print(f"{record[0]} Tickets: {record[1]}")
    else:
        print("No Bookings Yet")

    conn.close()

# Start Application
login()
