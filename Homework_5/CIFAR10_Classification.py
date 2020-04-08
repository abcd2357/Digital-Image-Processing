# EE569 Homework Assignment #5
# Problem 1 (b) CIFAR-10 Classification
# Name: XU Kangyan
# ID: 4876010998
# Email: kangyanx@usc.edu
# Date: Apr 7, 2020

import time
import torch
import torch.nn as nn
import torch.nn.functional as f
import torchvision
import torchvision.transforms as transforms
import torch.optim as optim
import matplotlib.pyplot as plt

# ------------------ Parameter ------------------ #
train_batch_size = 500  # 50000 images, batch-size 500, iteration 100.
test_batch_size = 100
conv_1_layer = 32
conv_2_layer = 64
spatial_size = 5
fc_1_layer = 200
fc_2_layer = 100
# ----------------------------------------------- #


class LeNet5(nn.Module):

    def __init__(self):
        super(LeNet5, self).__init__()
        # 1st convolution layer
        # input image - 3*32*32 / output - 32*28*28, 32 layers / Kernel Size - 5*5*3
        self.convolution_1 = nn.Conv2d(3, conv_1_layer, spatial_size)
        # 2nd convolution layer
        # input - 32*14*14 / output - 64*10*10, 64 layers / Kernel Size - 5*5*32
        self.convolution_2 = nn.Conv2d(conv_1_layer, conv_2_layer, spatial_size)
        # fully connected layer
        # 1st input 64*5*5
        self.fc_1 = nn.Linear(conv_2_layer*5*5, fc_1_layer)
        self.fc_2 = nn.Linear(fc_1_layer, fc_2_layer)
        self.fc_3 = nn.Linear(fc_2_layer, 10)

    def forward(self, x):
        # Max pooling
        # max(0, WTx+b)
        x = f.max_pool2d(f.relu(self.convolution_1(x)), (2, 2))
        x = f.max_pool2d(f.relu(self.convolution_2(x)), (2, 2))
        x = x.view(-1, self.num_flat_features(x))
        x = f.relu(self.fc_1(x))
        x = f.relu(self.fc_2(x))
        x = self.fc_3(x)
        return x

    def num_flat_features(self, x):
        size = x.size()[1:]
        num_features = 1
        for s in size:
            num_features *= s
        return num_features


def data_load():
    transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
    # 50000 / Download 1st time
    train_set = torchvision.datasets.CIFAR10(root='./data', train=True, download=True, transform=transform)
    train_loader = torch.utils.data.DataLoader(train_set, batch_size=train_batch_size, shuffle=True, num_workers=2)
    # 10000
    test_set = torchvision.datasets.CIFAR10(root='./data', train=False, download=True, transform=transform)
    test_loader = torch.utils.data.DataLoader(test_set, batch_size=test_batch_size, shuffle=False, num_workers=2)

    print("CIFAR10 Successfully Loaded!")
    return train_loader, test_loader


if __name__ == '__main__':
    train_loader, test_loader = data_load()
    network = LeNet5()

    # ------------------ Parameter ------------------ #
    epoch_times = 40
    optimizer_learning_rate = 0.001
    # ----------------------------------------------- #

    # Cross-entropy Loss / Adam
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(network.parameters(), lr=optimizer_learning_rate)

    # ----------------------------------------------- #
    train_epoch_accuracy = []
    test_epoch_accuracy = []

    for epoch in range(epoch_times):
        start_time = time.time()

        # ------------------ Training ------------------ #

        correct = 0
        total = 0
        for i, data in enumerate(train_loader, 0):

            inputs, labels = data
            # zero the parameter gradients
            optimizer.zero_grad()
            # forward + backward + optimize
            outputs = network(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()  # Update Weights

            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

        train_accuracy = 100 * correct / total
        print('Accuracy on 50000 train images: %d %%' % train_accuracy)

        # ------------------ Testing ------------------ #

        correct = 0
        total = 0
        with torch.no_grad():  # No Change to Weights
            for data in test_loader:
                images, labels = data
                outputs = network(images)
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()

        test_accuracy = 100 * correct / total
        print('Accuracy on 10000 test images: %d %%' % test_accuracy)

        # --------------------------------------------- #

        train_epoch_accuracy.append(train_accuracy)
        test_epoch_accuracy.append(test_accuracy)

        print('Epoch %d - %3f sec' % (epoch, time.time() - start_time))

    print("Training Finished")

    # ------------------ Accuracy Curves ------------------ #

    x1 = range(0, epoch_times)
    y1 = train_epoch_accuracy
    plt.plot(x1, y1, 'b-o')
    plt.title('Train Accuracy - Epoch')
    plt.xlabel('Epoch')
    plt.ylabel('Train Accuracy')
    plt.show()
    plt.savefig("Train Accuracy - Epoch.png")

    x2 = range(0, epoch_times)
    y2 = test_epoch_accuracy
    plt.plot(x2, y2, 'g-o')
    plt.title('Test Accuracy - Epoch')
    plt.xlabel('Epoch')
    plt.ylabel('Test Accuracy')
    plt.show()
    plt.savefig("Test Accuracy - Epoch.png")

    x3 = range(0, epoch_times)
    y2 = test_epoch_accuracy
    plt.plot(x3, y1, 'b-o', label='Train Accuracy')
    plt.plot(x3, y2, 'g-o', label='Test Accuracy')
    plt.title('Accuracy - Epoch')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.show()
    plt.savefig("Accuracy - Epoch.png")
