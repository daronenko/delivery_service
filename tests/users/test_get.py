async def test_get_user(service_client):
    request_body_1 = {
        'user': {
            'username': 'test_user',
            'email': 'example@mail.com',
            'user_type': 'courier',
            'password': '123456'
        }
    }

    response_1 = await service_client.post(
        'api/users',
        json=request_body_1
    )
    assert response_1.status == 200

    response_2 = await service_client.get(
        'api/user',
        bearer=response_1.json()['user']['token']
    )
    assert response_2.status == 200

    response_body_2 = response_2.json()

    assert response_body_2['user']['username'] \
        == request_body_1['user']['username']

    assert response_body_2['user']['email'] \
        == request_body_1['user']['email']

    assert response_body_2['user']['user_type'] \
        == request_body_1['user']['user_type']

    assert response_body_2['user']['token']
