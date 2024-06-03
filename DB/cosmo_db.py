import os
import uuid

from azure.cosmos import CosmosClient, PartitionKey
from dotenv import load_dotenv

load_dotenv()

# Cosmo DB Settings
ENDPOINT = os.getenv("ENDPOINT")
KEY = os.getenv("KEY")
DATABASE_ID = os.getenv("DATABASE_ID")
CONTAINER_ID = os.getenv("CONTAINER_ID")

# Initiate Cosmos Client
client = CosmosClient(ENDPOINT, KEY)
database = client.create_database_if_not_exists(id=DATABASE_ID)
container = database.create_container_if_not_exists(
    id=CONTAINER_ID,
    partition_key=PartitionKey(path="/userId"),
    offer_throughput=400
)

# Function to create account
def create_account(user_id, pin_code):
    account = {
        'id': str(uuid.uuid4()),
        'userId': user_id,
        'pinCode': pin_code
    }
    container.create_item(body=account)
    print(f"Created account for user: {user_id}")

# Create accounts
create_account("joakim", "1234")
create_account("leo", "5678")
create_account("jimmy", "0001")
create_account("admin", "9999")
