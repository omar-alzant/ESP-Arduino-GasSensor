from sqlite3.dbapi2 import Cursor
from flask import request,Flask,redirect,render_template
import os
import sqlite3


currlocation = os.path.dirname(os.path.abspath(__file__))

app = Flask(__name__)

@app.route("/")
def homepage():
    return render_template("homapage.html")

@app.route("/", methods= ["POST"])
def checklogin():
    UN = request.form['username']
    PW = request.form['password']

    sqlconnection = sqlite3.Connection(currlocation+"\login.db")
    cursor = sqlconnection.cursor()
    query1 = "SELECT username, password FROM Users WHERE username = {un} AND password = {pw})".format(un = UN, pw = PW)

    rows = cursor.execute(query1)
    rows = rows.fetchall()
    if len(rows) == 1:
        return render_template("index.html")
    else :
        return redirect("/register")  

app.route("/register", methods =["POST","GET"])
def registerpage():
    if request.methods == "POST" :
        dUN = request.form['DUsername']
        dPW = request.form['DPassword']
        UEmail = request.form['EmailUser']
        sqlconnection = sqlite3.Connection(currlocation + "\login.db")
        cursor = sqlconnection.cursor()
        query1 = "INSERT INTO Users VALUE ('{u}', '{p}', '{e}')".format(u = dUN, p = dPW, e = UEmail)
        cursor.execute(query1)
        sqlconnection.commit()
        return redirect("/")
    return render_template("register.html")    


if __name__ == '__main__':
    app.run()