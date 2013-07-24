'''
Created on May 25, 2012

@author: Cengiz
'''

class BadInputError(Exception):
    
    def __init__(self, error):
        self.error=error
    def __str__(self):
        print(self.error)