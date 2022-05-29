# Linda

## Quick start

### Ubuntu

```sh
sudo apt-get update
sudo apt-get install cmake ninja-build libboost-all-dev
sudo make prepare build test
```

### Other systems

1. [Install Cmake](https://cgold.readthedocs.io/en/latest/first-step/installation.html)

2. [Install Ninja](https://ninja-build.org)

3. Install boost

4. Run Make command

    ```sh
    sudo make prepare build test
    ```

## Others

- In order to run application you need to be a superuser. It's necessary for the communication via FIFO.
- Pipes are stored at: /tmp. But you can edit it in the CommunicationService
- Testing framework: [doctest](https://github.com/doctest/doctest)

### How to run an agent scenario

- Create a json file under the root project directory e.x. `scenario.json`
    - Json example: [Link](https://gist.github.com/pochka15/a4b759f58e4e5a66e26fec74d57bf633)
- When running writer or reader set the parameter `--scenario scenario.json`
- Note: you can also provide with a full path to the scenario `--scenario /tmp/someScenario.json`
