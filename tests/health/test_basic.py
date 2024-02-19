# Start the tests via `make test-debug` or `make test-release`


async def test_health(service_client):
    response = await service_client.get('/api/health')
    assert response.status == 200
    assert response.text == 'OK\n'
