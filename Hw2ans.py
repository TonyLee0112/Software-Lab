import sklearn
import matplotlib.pyplot as plt
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

train_percent = 0.4
valid_percent = train_percent * 0.1

# Modify the data location for loading data
data = np.loadtxt('data.csv', unpack=True, delimiter=',', skiprows=0)
data = np.transpose(data)

# Data shuffle
np.random.shuffle(data)

# Obtaining Training data set
train_end = int(len(data) * train_percent)
valid_end = train_end + int(len(data) * valid_percent)

train_set = data[:train_end]
train_set = sorted(train_set, key=lambda train_set: train_set[0])  # Sorting again for data in order
train_set = np.transpose(train_set)

# Re-allocate for efficient programming
train_x = train_set[0]  # train_set[0]: feature data set (i.e., x)
train_y = train_set[1]  # train_set[1]: label data set (i.e., y)

# Write code for obtaining valid data set: valid_set
valid_set = data[train_end:valid_end]
valid_set = sorted(valid_set, key=lambda valid_set: valid_set[0])
valid_set = np.transpose(valid_set)

valid_x = valid_set[0]
valid_y = valid_set[1]

# Write code for obtaining test data set: test_set
test_set = data[valid_end:]
test_set = sorted(test_set, key=lambda test_set: test_set[0])
test_set = np.transpose(test_set)

test_x = test_set[0]
test_y = test_set[1]

##################### Regression Libraries #############
def fit_polynomial(x, y, degree):
    '''
    Fits a polynomial to the input sample.
    (x,y): input sample
    degree: polynomial degree
    '''
    model = LinearRegression()
    model.fit(np.vander(x, degree + 1), y) # 다항식의 계수 반환
    return model

def apply_polynomial(model, x):
    '''
    Evaluates a linear regression model in an input sample
    model: linear regression model
    x: input sample
    '''
    degree = model.coef_.size - 1
    y = model.predict(np.vander(x, degree + 1))
    return y
##################### End of Regression Libraries #############

# Starting values
Optimal_Order = 0
Minimum_MSE = 9999
Optimal_Model = None

# Determine minimum MSE for valid set as increasing polynomial order from 1 to 10.
for polynomial_order in range(1, 10):
    model = fit_polynomial(train_x, train_y, polynomial_order)
    Estimated_valid_y = apply_polynomial(model, valid_x)

    # Write codes measuring MSE for valid set
    # For calculating MSE use the library "mean_squared_error" in "sklearn.metrics"
    MSE = mean_squared_error(valid_y, Estimated_valid_y)

    if Minimum_MSE > MSE:
        Optimal_Order = polynomial_order
        Minimum_MSE = MSE
        Optimal_Model = model

print("----------------------", "\n")
print("We can choose best polynomial order with MSE of validation set.")
print("Optimal order is ", Optimal_Order)
print("Minimum MSE is ", Minimum_MSE)
print("\n")
print("-----TEST RESULT-----")

# Overlay Regression polynomial along training feature data and test data set (test_x, test_y)
plt.plot(train_x, apply_polynomial(Optimal_Model, train_x), 'g')  # Display with lines colored with green (g).
plt.plot(test_x, test_y, 'b.')  # Display with dots colored with blue (b).

# Write code for calculating MSE performance of the Optimal regression polynomial.
Estimated_test_y = apply_polynomial(Optimal_Model, test_x)
MSE_Performance = mean_squared_error(test_y, Estimated_test_y)

print("MSE : ", MSE_Performance)
print("The coefficient of model is ", Optimal_Model.coef_, ", ", Optimal_Model.intercept_)

plt.xlabel('Feature values : x')
plt.ylabel('Label values : y')
plt.grid()
plt.suptitle('Polynomial Regression', fontsize=16)
plt.show()
