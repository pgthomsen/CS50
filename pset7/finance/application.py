from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """ get the current user's holdings information"""
    if session["user_id"] is None:
        return render_template("landing.html")
    user = db.execute("SELECT * FROM users WHERE id=:user_id;", user_id=session["user_id"])
    holdings = db.execute("SELECT * FROM holdings WHERE user_id=:user_id;", user_id=session["user_id"])
    totalValue=0
    values = [len(holdings)]
    """ go through each stock and get its current list price and the value of all shares, storing in the holdings list """
    for i in range(len(holdings)):
        stock = lookup(holdings[i]['ticker'])
        worth = stock['price'] * holdings[i]['quantity']
        holdings[i].update({"value":usd(worth)})
        holdings[i].update({"pps":usd(stock['price'])})
        totalValue = totalValue + worth
        
    total= totalValue + user[0]['cash']
    if totalValue == 0:
        return render_template("index.html", cash = usd(user[0]['cash']),holding_value=usd(totalValue), total=usd(total))
    
    return render_template("index.html", stocks=holdings, worths=values, cash=usd(user[0]['cash']),\
    holding_value=usd(totalValue), total=usd(total))
    
@app.route("/landing")
def landing():
    """ get the current user's holdings information"""
    return render_template("landing.html")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    """Ensure that valid inputs are provided"""
    if request.method == "POST":
        if not request.form.get("ticker"):
            return apology("Please enter a ticker")
            
        elif not request.form.get("quantity"):
            return apology("Please enter a quantity")
        
        ticker = request.form.get("ticker")
        
        if not ticker.isalpha() or len(ticker) > 4:
            return apology("please enter a properly formated ticker")
        
        if not request.form.get("quantity").isnumeric():
            return apology("Please enter a number only!")
        
        ticker.upper()
        
        sharePrice = lookup(ticker)
        
        if sharePrice == None:
            return apology("ticker has no match")
            
        """ Make sure user has enough money to complete transaction, then complete transaction """
        shareCount = int(request.form.get("quantity"))
        
        if not shareCount > 0 or shareCount != int(shareCount):
            return apology("Please enter a positive integer share number")
        
        rows = db.execute("SELECT * FROM holdings WHERE user_id = :userid AND ticker = :tickr;",\
        userid=session["user_id"], tickr=ticker)
        
        rows2 = db.execute("SELECT cash FROM users WHERE id = :user_id;", user_id=session["user_id"])
        
        if not rows2[0]["cash"] > sharePrice['price'] * shareCount:
            return apology("Not enough cash to complete the Transaction")
            
        totalCost = sharePrice['price'] * shareCount
        currentCash = rows2[0]["cash"] - totalCost
        
        if len(rows) == 0:
            db.execute("INSERT INTO holdings (user_id, ticker, quantity) VALUES(:user_id, :tickr, :quantity);", \
            user_id=session["user_id"], tickr=ticker, quantity=shareCount)
            
        else:
            db.execute("UPDATE holdings SET quantity=:quantity WHERE user_id=:user_id AND ticker=:tickr;",\
            quantity=rows[0]["quantity"]+shareCount, user_id=session["user_id"], tickr=ticker)
            
        db.execute("INSERT INTO transactions (ticker, quantity, price, user_id, transaction_type) VALUES(:tickr, :quantity, :price, :user_id, 'BUY');",\
        tickr=ticker, quantity=shareCount, price=sharePrice['price'], user_id=session["user_id"])
        
        db.execute("UPDATE users SET cash=:current_cash WHERE id=:user_id;", current_cash=currentCash, user_id=session["user_id"])
        return render_template("buy_success.html", stock_name=sharePrice['name'], ticker=sharePrice['symbol'], price= sharePrice['price'],\
        total=totalCost)
            
        
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    hist = db.execute("SELECT * FROM transactions WHERE user_id=:user_id;",user_id=session["user_id"])
    for i in range(len(hist)):
        price = hist[i]['price']
        hist[i].update({'usd_price':usd(price)})
    return render_template("history.html", history=hist)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username;", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        if len(request.form.get("ticker")) > 4 or not request.form.get("ticker").isalpha():
            return apology("Please enter a correctly formated ticker")
            
        elif not request.form.get("ticker").isalpha():
            return apology("Please enter a correctly formated ticker")
        
        stock = lookup(request.form.get("ticker"))
        
        if stock == None:
            return apology("Ticker not found")
        
        return render_template("quoted.html", ticker=stock['symbol'], stock_name=stock['name'], price=usd(stock['price']))
        
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()

    # if user reached route via POST
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
        
        rows = db.execute("SELECT * FROM users WHERE username = :username;", username=request.form.get("username"))
        
        if len(rows) != 0:
            return apology("Username already in use")
            
        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        
        elif request.form.get("password") != request.form.get("password2"):
            return apology("Passwords must match")
        
        hashvalue = pwd_context.encrypt(request.form.get("password"))
        
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :password);", username=request.form.get("username"), password=hashvalue)
        
        rows = db.execute("SELECT * FROM users WHERE username = :username;", username=request.form.get("username"))

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        if not request.form.get("ticker"):
            return apology("Please enter a ticker")
            
        elif not request.form.get("quantity"):
            return apology("Please enter a quantity")
        
        ticker = request.form.get("ticker")
        
        if not ticker.isalpha() or len(ticker) > 4:
            return apology("please enter a properly formated ticker")
            
        if not request.form.get("quantity").isnumeric():
            return apology("Please enter a number only!")
        
        ticker.upper()
        
        sharePrice = lookup(ticker)
        if sharePrice == None:
            return apology("Ticker not found")
            
        shareCount = int(request.form.get("quantity"))
        
        if not shareCount > 0 or shareCount != int(shareCount):
            return apology("Please enter a positive integer share number")
        
        rows = db.execute("SELECT * FROM holdings WHERE user_id = :userid AND ticker = :tickr;", userid=session["user_id"], tickr=ticker)
        
        rows2 = db.execute("SELECT cash FROM users WHERE id = :user_id;", user_id=session["user_id"])
        
        if not rows[0]["quantity"] >= shareCount:
            return apology("Not enough shares to complete the Transaction")
            
        totalValue = sharePrice['price'] * shareCount
        currentCash = rows2[0]["cash"] + totalValue
        
        if len(rows) == 0:
            return apology("Sorry but you do not own any shares of this stock!")
            
        else:
            db.execute("UPDATE holdings SET quantity=:quantity WHERE user_id=:user_id AND ticker=:tickr;",quantity=rows[0]["quantity"]-shareCount,\
            user_id=session["user_id"], tickr=ticker)
            
        db.execute("INSERT INTO transactions (ticker, quantity, price, user_id, transaction_type) VALUES(:tickr, :quantity, :price, :user_id, 'SELL');",\
        tickr=ticker, quantity=shareCount, price=sharePrice['price'], user_id=session["user_id"])
        
        db.execute("UPDATE users SET cash=:current_cash WHERE id=:user_id;", current_cash=currentCash, user_id=session["user_id"])
        if (rows[0]["quantity"] - shareCount) == 0:
            db.execute("DELETE FROM holdings WHERE ticker=:tickr", tickr=ticker)
        
        return render_template("sell_success.html", stock_name=sharePrice['name'], ticker=sharePrice['symbol'], price= sharePrice['price'],\
        total=totalValue)
            
        
    else:
        return render_template("sell.html")

@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """ add cash to account """
    if request.method == "POST":
        if not request.form.get("cash_add"):
            return apology("Please enter a cash value to add to your account")
            
        if not float(request.form.get("cash_add")) > 0:
            return apology("Please enter a positive cash value to add to your account")
            
        if not request.form.get("cash_add").isnumeric():
            return apology("Please enter a number only!")
        
        current = db.execute("SELECT cash FROM users WHERE id=:user_id;", user_id=session["user_id"])
        newCash = current[0]['cash'] + float(request.form.get("cash_add"))
        db.execute("UPDATE users SET cash=:new WHERE id=:user_id;", new=newCash, user_id=session["user_id"])
        return render_template("currentcash.html", cash=newCash)
        
    else:
        return render_template("addbalance.html")