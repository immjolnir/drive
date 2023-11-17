# -*- coding: utf-8 -*-

# https://pytorch.org/tutorials/beginner/pytorch_with_examples.html
# Numpy provides an n-dimensional array object, and many functions for manipulating these arrays.
# Numpy is a generic framework for scientific computing; it does not know anything about computation graphs, or deep learning, or gradients.
# However we can easily use numpy to fit a third order polynomial to sine function by manually implementing the forward and backward passes through the network using numpy operations:
import numpy as np
import math

# Create random input and output data
x = np.linspace(-math.pi, math.pi, 2000) # input
y = np.sin(x)                            # model

# Randomly initialize weights
a = np.random.randn()
b = np.random.randn()
c = np.random.randn()
d = np.random.randn()

print(f'Weights: a={a}, b={b}, c={c}, d={d}')

learning_rate = 1e-6  # Try to adjust it?
for t in range(2000): # Try to adjust the loops?
    # Forward pass: compute predicted y
    # y = a + b x + c x^2 + d x^3
    y_pred = a + b * x + c * x ** 2 + d * x ** 3

    # Compute and print loss
    loss = np.square(y_pred - y).sum()
    if t % 100 == 99:
        print(t, loss)

    # Backprop to compute gradients of a, b, c, d with respect to loss
    grad_y_pred = 2.0 * (y_pred - y)
    grad_a = grad_y_pred.sum()
    grad_b = (grad_y_pred * x).sum()
    grad_c = (grad_y_pred * x ** 2).sum()
    grad_d = (grad_y_pred * x ** 3).sum()

    # Update weights
    a -= learning_rate * grad_a
    b -= learning_rate * grad_b
    c -= learning_rate * grad_c
    d -= learning_rate * grad_d

print(f'Result: y = {a} + {b} x + {c} x^2 + {d} x^3')