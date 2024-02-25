async def test_login(service_client):
    request_body_1 = {
        'user': {
            'username': 'test_user',
            'email': 'example@mail.com',
            'user_type': 'regular',
            'password': '123456'
        }
    }

    response_1 = await service_client.post(
        'api/users',
        json=request_body_1
    )
    assert response_1.status == 200

    request_body_2 = {
        'user': {
            'email': 'example@mail.com',
            'password': '123456'
        }
    }

    response_2 = await service_client.post(
        'api/users/login',
        json=request_body_2,
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


async def test_login_invalid_password(service_client):
    request_body_1 = {
        'user': {
            'username': 'test_user',
            'email': 'example@mail.com',
            'user_type': 'regular',
            'password': '123456'
        }
    }

    response_1 = await service_client.post(
        'api/users',
        json=request_body_1
    )
    assert response_1.status == 200

    request_body_2 = {
        'user': {
            'email': 'example@mail.com',
            'password': 'some_invalid_password'
        }
    }

    response_2 = await service_client.post(
        'api/users/login',
        json=request_body_2,
        bearer=response_1.json()['user']['token']
    )
    assert response_2.status == 404
