# Azure Cosmos DB Python Project

This project demonstrates how to use Azure Cosmos DB with a Python application to create and manage accounts. The project uses environment variables to handle sensitive information such as the Cosmos DB endpoint and key.

## Requirements

- Python 3.7 or higher
- An Azure Cosmos DB instance
- A `.env` file to store sensitive information

## Installation

1. Create a virtual environment and activate it:

   ```sh
   python -m venv venv # or python3 -m venv venv for 3.7 or higher
   source venv/bin/activate  # On Windows use: venv\Scripts\activate
   ```

2. Install dependencies:

   ```sh
   pip install -r requirements.txt
   ```

## .env File

Create a `.env` file in the root directory of the project with the following content:

```env
ENDPOINT=https://your-cosmos-account.documents.azure.com:443/
KEY=your_primary_key
DATABASE_ID=your_database_id
CONTAINER_ID=your_container_id
```
