import torch
import torch.nn as nn
import torchvision.datasets as datasets
import torchvision.transforms as transforms
import torch.optim as optim

# define MyLeNet
class LeNet(nn.Module):
  def __init__(self):
    super(LeNet, self).__init__()
    self.conv_1 = nn.Conv2d(1,6,kernel_size=5,padding=2) # grayscale 28*28*1 N개, zero padding = 2
    self.maxpool_1 = nn.MaxPool2d(kernel_size=(2,2), stride = 2)
    self.conv_2 = nn.Conv2d(6, 16, kernel_size=5)
    self.max_pool_2 = nn.MaxPool2d(kernel_size=(2,2), stride = 2)
    self.conv_3 = nn.Conv2d(16,120, kernel_size=5)
    self.relu = nn.ReLU()
    self.fc_1 = nn.Linear(120,84) # Fully Connected Layer
    self.fc_2 = nn.Linear(84,10)

  def forward(self, x):
    x = self.conv_1(x)
    x = self.relu(x) # CNN layer 이후 Activation function 섞어주기
    x = self.maxpool_1(x)
    x = self.conv_2(x)
    x = self.relu(x)
    x = self.max_pool_2(x)
    x = self.conv_3(x)
    x = x.view(-1,120) # flatten
    x = self.fc_1(x)
    x = self.relu(x)
    x = self.fc_2(x)
    return x
    
# Model Creation and Set up Hyper-parameters
batch_size = 16
learning_rate = 0.1
epochs = 10

use_cuda = torch.cuda.is_available()
device = torch.device("cuda" if use_cuda else "cpu")

model = LeNet().to(device)

kwargs = {'num_workers':1, 'pin_memory':True} if use_cuda else {}

train_dataset = datasets.MNIST('./data', train = True, download = True, transform = transforms.Compose([transforms.ToTensor(),transforms.Normalize((0.1307,),(0.3081,))]))
train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True, **kwargs)
test_dataset = datasets.MNIST('./data', train=False, transform=transforms.Compose([transforms.ToTensor(),transforms.Normalize((0.1307,),(0.3081,))]))
test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=batch_size,shuffle=True, **kwargs)

print('[Trainset: ]',train_dataset)
print('[Testset: ]', test_dataset)

# Optimization
optimizer = optim.Adadelta(model.parameters(), lr=learning_rate)
scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=1, gamma=0.1)
loss_function = nn.CrossEntropyLoss()

# define Train function
def train(model, device, train_loader, optimizer, loss_function, epoch):
  model.train()
  for batch_idx, (data, target) in enumerate(train_loader):
    data, target = data.to(device), target.to(device)
    optimizer.zero_grad()
    output = model(data)
    loss = loss_function(output, target)
    loss.backward()
    optimizer.step()

    if batch_idx % 1000 == 0:
      print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss:{:.6f}'.format(epoch, batch_idx * len(data), len(train_loader.dataset), 100. * batch_idx / len(train_loader), loss.item()))

# define Test function
def test(model, device, test_loader, loss_function) :
  model.eval()
  test_loss = 0
  correct = 0

  with torch.no_grad(): #
    for data, target in test_loader:
      data, target = data.to(device), target.to(device)
      output = model(data)
      test_loss += loss_function(output, target)
      pred = output.argmax(dim=1, keepdim=True)
      correct += pred.eq(target.view_as(pred)).sum().item()

  test_loss /= len(test_loader.dataset)
  print('\nTest set: Average loss: {:.4f}, Accuracy: {}/{} ({:.0f}%)\n'.format(test_loss, correct, len(test_loader.dataset), 100. * correct / len(test_loader.dataset)))

# run test
for epoch in range(epochs) :
  train(model, device,train_loader, optimizer, loss_function, epoch)
  test(model, device, test_loader, loss_function)
  scheduler.step()
