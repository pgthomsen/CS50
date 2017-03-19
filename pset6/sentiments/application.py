from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name")
    if not screen_name:
        return redirect(url_for("index"))

    analyzer = Analyzer()
    # get screen_name's tweets, returning an the index page if unable to reach profile
    try:
        tweets = helpers.get_user_timeline(screen_name, 100)
    except Exception:
        return render_template("index.html")

    # initialize the counts for how many tweets are positive, negative, and neutral
    positiveCount = 0
    negativeCount = 0
    neutralCount = 0
    
    # check each tweet, increasing the count of its repective classification
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        
        if score > 0:
            positiveCount += 1
        elif score < 0:
            negativeCount += 1
        elif score == 0:
            neutralCount += 1
    
    # calculate the percentage for the counts
    totalCount = positiveCount + negativeCount + neutralCount
    
    positive = (positiveCount / totalCount) * 100
    
    negative = (negativeCount / totalCount) * 100
    
    neutral = (neutralCount / totalCount) * 100

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
