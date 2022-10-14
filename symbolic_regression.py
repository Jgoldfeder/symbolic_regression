import random, math
import time
import matplotlib.pyplot as plt
import numpy as np
import ast,copy
import sys

def read_data(dataset):
    count = 0

    points = []

    with open("data2022_"+dataset+".txt") as fp:
        Lines = fp.readlines()
        for line in Lines:
            toks = line.split(",")
            point = (float(toks[0]), float(toks[1]))
            count += 1
            points.append(point)
            
    print("read " + str(count) + " points from file!")
    return points
    
def get_tree(string):
    ast_exp = ast.parse(string)
    return ast_exp
    
def get_string(tree):
    return ast.unparse(tree)
    
def evaluate_tree(tree,x):
    x=x
    # if divide by zero return zero
    try:
        return eval(ast.unparse(tree))
    except ZeroDivisionError:
        return 0
        
def get_error(tree,points):
    dist = 0
    for p in points:
        dist += abs(evaluate_tree(tree,p[0]) - p[1])
    return dist

def get_random_expression(max_depth=random.randint(0,4)):
    type = random.choices(["terminal","binary_operator","function"],weights =[0.2,0.7,.1])[0]
    if type == "terminal" or max_depth==0:
       return random.choice([str(random.randrange(-10, 10)),"x","x*x"])
    elif type == "binary_operator":
        operator = random.choice(["+","-","/","*"])
        left = get_random_expression(max_depth-1)
        right = get_random_expression(max_depth-1)
        return "("+ left + operator + right + ")"        
    elif type == "function":
        f = random.choice(["math.cos","math.sin"])
        return f + "(" + get_random_expression(max_depth-1) + ")"

types = {}
types["binary_operator"] = "<class 'ast.BinOp'>"
types["constant"] = "<class 'ast.Constant'>"
types["variable"] = "<class 'ast.Name'>"
types["function"] = "<class 'ast.Call'>"
        
def mutate(tree,points):
    # copy for safety
    tree = copy.deepcopy(tree)
    
    root = tree.body[0].value
    constants = []
    operators = []
    variables = []
    functions = []
    
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
    
    
datasets = ["None","Gold","Silver","Bronze"]
points = read_data(datasets[int(sys.argv[1])])

start = time.time()

population_size = 10
population = []

best_tree = None
best_error = 10000000

for i in range(population_size):
    exp = "-1*x*x+10*x+10"
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
#for i in range(10):
#    print(get_random_expression())