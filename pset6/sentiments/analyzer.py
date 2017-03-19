import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives="positive-words.txt", negatives="negative-words.txt"):
        """Initialize Analyzer."""

        pos = open(positives)
        self.poslist = []
        # from python documentation, go through each line in a file and, if it is a word, add to the poslist list
        for line in pos:
            if line[0] != ';' and line[0] != '\n':
                self.poslist.append(line.rstrip())
        # Go through each line in a file and, if it is a word, add to the neglist list
        neg = open(negatives)
        self.neglist = []
        for line in neg:
            if line[0] != ';' and line[0] != '\n':
                self.neglist.append(line.rstrip())

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # tokenize the string tweet into a list of words/tokens
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        count = 0
        
        # check each token against the two lists, adjusting the tweet's count as needed
        for i in range(len(tokens)):
            word = tokens[i].lower()
            if word in self.poslist:
                count += 1
            elif word in self.neglist:
                count -= 1
                
        #return the tweet's score
        return count
