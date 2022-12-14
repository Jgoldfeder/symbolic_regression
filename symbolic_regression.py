import random, math
import time
import matplotlib.pyplot as plt
import numpy as np
import ast,copy
import sys

# Using the sqrt() function to calculate a square root
from math import sqrt

def read_data(dataset): #PASSES ARGUMENT GOLD, BRONZE, OR SILVER AND READS DATASET
    count = 0 

    points = [] #LIST OF RAW INPUT/OUTPUT DATA PAIRS, LIST OF TUPLES

    with open("data2022_"+dataset+".txt") as fp: #READ DATASET
        Lines = fp.readlines()
        for line in Lines:
            toks = line.split(",")
            point = (float(toks[0]), float(toks[1]))
            count += 1
            points.append(point)
            
    print("read " + str(count) + " points from file!")
    return points
    
def get_tree(string): #TAKES IN EQUATION STRING EXPRESSION AND RETURNS TREE 
    ast_exp = ast.parse(string)
    return ast_exp
    
def get_string(tree): #CONVERTS BINARY/SYNTAX TREE (AST) INTO STRING EQUATION EXPRESSION
    return ast.unparse(tree)
    
def evaluate_tree(tree,x): #EVALUATES TREE, EXECUTES EQUATION EXPRESSION FOR f(x)
    x=x
    # if divide by zero return zero
    try:
        return eval(ast.unparse(tree))
    except ZeroDivisionError: #TODO: imaginary number return 0?
        return 0
    except ValueError:
        return 0
        
def get_error(tree,points): #FINDS ABSOLUTE ERROR BETWEEN TREE EXPRESSION FUNCTION AND ACTUAL RAW DATA
    dist = 0
    for p in points:
        dist += abs(evaluate_tree(tree,p[0]) - p[1])
    return dist

def get_random_expression(max_depth=random.randint(0,4)): #GENERATES RANDOM FUNCTION STRING, RANDOM DEPTH GENERATED BY DEFAULT IN RANGE 
    type = random.choices(["terminal","binary_operator","function"],weights =[0.2,0.7,.1])[0] #INCL. WEIGHTS FOR SPAWNING EACH EXPRESSION TYPE
    #TERMINAL: Leaf node (constant or variable)
    #BINARY OPERATOR: Node with 2 output (2 outputs)
    #FUNCTION: Unary operator (1 output)
    if type == "terminal" or max_depth==0:
        #Constant/Variable Spawn Range (-10,10)
        return random.choice([str(random.randrange(-10, 10)),"x","x*x"]) 
    elif type == "binary_operator":
        operator = random.choice(["+","-","/","*"])
        left = get_random_expression(max_depth-1) #MAX DEPTH REDUCED FOR RECURSIVE CALL (standard recursion)
        right = get_random_expression(max_depth-1)
        return "("+ left + operator + right + ")"        
    elif type == "function":
        f = random.choice(["math.cos","math.sin"]) #TODO: add "math.exp"
        return f + "(" + get_random_expression(max_depth-1) + ")"

#EACH NODE OF TREE IS ONE OF THESE OPERATORS 
types = {}
types["binary_operator"] = "<class 'ast.BinOp'>" #TWO CHILDREN --> Access with node.left and node.right
types["constant"] = "<class 'ast.Constant'>" #NO CHILDREN
types["variable"] = "<class 'ast.Name'>" #NO CHILDREN
types["function"] = "<class 'ast.Call'>" #ONE CHILD --> Access with node.args[0]
        
def mutate(tree,points):
    # copy for safety
    tree = copy.deepcopy(tree)
    
    root = tree.body[0].value
    constants = []
    operators = []
    variables = []
    functions = []
    
    #DEPTH FIRST SEARCH ALGORITHM FOR TREE TRAVERSAL
    queue = [root]
    while queue:
        node = queue.pop()
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            operators.append((node,root))
            queue.append(node.left)
            queue.append(node.right)
            root = node
            continue 
        elif node_type == types["constant"]:
            constants.append((node,root))
            root = node
            continue
        elif node_type == types["variable"]:
            variables.append((node,root))
            root = node
            continue            
        elif node_type == types["function"]:
            functions.append((node,root))
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            root = node            
            continue 
    
    # mutate constants
    num_constant_mutations = 10
    if len(constants) > 0:
        for _ in range(num_constant_mutations):
            # first pick a constant at random
            constant = random.choice(constants)[0]
            
            # give it a random value and keep if better
            original_value = constant.value
            error = get_error(tree,points)
            constant.value = random.randrange(-10, 10)
            if get_error(tree,points) > error:
                # revert
                constant.value = original_value 
            
    return tree
    
def crossover(tree):

    
    return tree


datasets = ["None","Gold","Silver","Bronze"]
points = read_data(datasets[int(sys.argv[1])])

start = time.time()

population_size = 10
population = []

best_tree = None
best_error = 10000000

for i in range(population_size):
    exp = get_random_expression()
    tree = get_tree(exp)
    original_error = get_error(tree,points)
    tree = mutate(tree,points)
    new_error = get_error(tree,points)
    print(i,original_error,new_error, original_error-new_error,get_string(tree))
    population.append(tree)
    if new_error < best_error:
        best_error = new_error
        best_tree = tree
        
#print(sqrt(-1))

x=[]
y=[]
for p in points:
    x.append(p[0])
    y.append(p[1])
y_2 = []
for p in points:
    y_2.append(evaluate_tree(best_tree,p[0]))    
 
print("time elapsed:", time.time() - start,"best error:", get_error(best_tree,points), "f(x)=", get_string(best_tree))
plt.plot(x,y)
plt.plot(x,y_2)
plt.show()    
for i in range(10):
    print(get_random_expression())