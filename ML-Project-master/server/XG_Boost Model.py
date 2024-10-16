import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score
from sklearn.preprocessing import LabelEncoder
import xgboost as xgb
import joblib

# Load the datasets
exercise_file_path = '/content/exercise(1).csv'
calories_file_path = '/content/calories(1).csv'

exercise_data = pd.read_csv(exercise_file_path)
calories_data = pd.read_csv(calories_file_path)

# Merge the datasets on user_id
merged_data = exercise_data.merge(calories_data, on='User_ID')

# Preprocess data
label_encoder = LabelEncoder()
merged_data['Gender'] = label_encoder.fit_transform(merged_data['Gender'])

# Features and target variable
X = merged_data.drop(['User_ID', 'Calories'], axis=1)  # Dropping 'userid' and 'calories' as it's the target
y = merged_data['Calories']  # Target variable

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train XGBoost model
xg_reg = xgb.XGBRegressor(n_estimators=100, learning_rate=0.1, max_depth=5, random_state=42)
xg_reg.fit(X_train, y_train)

# Make predictions
y_pred = xg_reg.predict(X_test)

# Evaluate the model
mse = mean_squared_error(y_test, y_pred)
mae = mean_absolute_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)
mape = np.mean(np.abs((y_test - y_pred) / y_test)) * 100

# Print evaluation metrics
print(f"Mean Squared Error (MSE): {mse}")
print(f"Mean Absolute Error (MAE): {mae}")
print(f"R-squared (R²): {r2}")
print(f"Mean Absolute Percentage Error (MAPE): {mape:.2f}%")

# Plot actual vs predicted values
plt.figure(figsize=(10, 6))
plt.scatter(y_test, y_pred, edgecolor='k', alpha=0.7)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--', lw=2)
plt.xlabel('Actual Calories')
plt.ylabel('Predicted Calories')
plt.title('Actual vs Predicted Calories (XGBoost)')
plt.grid(True)
plt.show()
