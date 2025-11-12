import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv')

plt.figure(figsize=(12, 6))
for region in df['region'].unique():
    subset = df[df['region'] == region]
    plt.plot(subset['N'], subset['estimated_area'], label=f'{region} область')
plt.axhline(y=df['exact_area'].iloc[0], color='red', linestyle='--', label='Точное значение')
plt.xlabel('Количество точек N')
plt.ylabel('Оценка площади')
plt.title('Зависимость оценки площади от количества точек и области')
plt.legend()
plt.grid(True)
plt.savefig('area_estimates.png', dpi=300, bbox_inches='tight')
plt.show()

plt.figure(figsize=(12, 6))
for region in df['region'].unique():
    subset = df[df['region'] == region]
    plt.plot(subset['N'], subset['relative_error'] * 100, label=f'{region} область')
plt.xlabel('Количество точек N')
plt.ylabel('Относительная ошибка (%)')
plt.title('Зависимость относительной ошибки от количества точек и области')
plt.legend()
plt.grid(True)
plt.savefig('relative_error.png', dpi=300, bbox_inches='tight')
plt.show()