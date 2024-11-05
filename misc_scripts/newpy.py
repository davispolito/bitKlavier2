import requests
from bs4 import BeautifulSoup

url = 'https://docs.juce.com/master/classes.html'
response = requests.get(url)
soup = BeautifulSoup(response.content, 'html.parser')

# Find all links to class documentation
class_links = soup.find_all('a', class_='el')

# Extract class names
class_names = [link.text for link in class_links]

# Write class names to a file
with open('class_names.txt', 'w') as file:
    for name in class_names:
        file.write(f"{name}\n")

print("Class names extracted and saved to class_names.txt")

