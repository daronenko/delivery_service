async def test_register(service_client):
    request_body = {
        'user': {
            'username': 'test_user',
            'email': 'example@mail.com',
            'user_type': 'regular',
            'password': '123456'
        }
    }

    response = await service_client.post(
        'api/users',
        json=request_body
    )
    assert response.status == 200

    response_body = response.json()

    assert response_body['user']['username'] \
        == request_body['user']['username']

    assert response_body['user']['email'] \
        == request_body['user']['email']

    assert response_body['user']['user_type'] \
        == request_body['user']['user_type']

    assert response_body['user']['token']


async def test_register_same_username(service_client):
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
            'username': 'test_user',
            'email': 'another@mail.com',
            'user_type': 'regular',
            'password': 'qwerty'
        }
    }

    response_2 = await service_client.post(
        'api/users',
        json=request_body_2
    )
    assert response_2.status == 422
    assert 'errors' in response_2.json()


async def test_register_same_email(service_client):
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
            'username': 'another_test_user',
            'email': 'example@mail.com',
            'user_type': 'regular',
            'password': 'qwerty'
        }
    }

    response_2 = await service_client.post(
        'api/users',
        json=request_body_2
    )
    assert response_2.status == 422
    assert 'errors' in response_2.json()


async def test_register_same_password(service_client):
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
            'username': 'another_test_user',
            'email': 'another@mail.com',
            'user_type': 'regular',
            'password': '123456'
        }
    }

    response_2 = await service_client.post(
        'api/users',
        json=request_body_2
    )
    assert response_2.status == 200
