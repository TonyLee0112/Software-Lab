import os
import torch
import torchvision
import torch.nn as nn
from torchvision import transforms
from torchvision.utils import save_image

# Device Configuration
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# Hyper-Parameter set up
latent_size = 64 # Random vector size
hidden_size = 256
image_size = 28*28
batch_size = 100
epochs = 100
sample_dir = 'sample_data'
learning_rate = 0.0002

# Create a directory if not exists
if not os.path.exists(sample_dir) :
  os.makesdirs(sample_dir)

# Image Processing : 이미지를 Pytorch Tensor 로 변환
transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize(mean=[0.5], std=[0.5])])

# MNIST dataset download
mnist = torchvision.datasets.MNIST(root='data/', train=True, transform=transform, download=True)

# DataLoader
data_loader = torch.utils.data.DataLoader(dataset=mnist, batch_size=batch_size, shuffle=True)

# Discriminator model 생성
D = nn.Sequential(
    nn.Linear(image_size, hidden_size), # 768 -> 256
    nn.LeakyReLU(0.2),
    nn.Linear(hidden_size, hidden_size), # 256 -> 256
    nn.LeakyReLU(0.2),
    nn.Linear(hidden_size, 1), # 256 -> 1
    nn.Sigmoid() # Discriminator 의 핵심
)

# Generator model 생성
G = nn.Sequential(
    nn.Linear(latent_size,hidden_size),
    nn.ReLU(),
    nn.Linear(hidden_size, hidden_size),
    nn.ReLU(),
    nn.Linear(hidden_size, image_size),
    nn.Tanh()
)

# Device Setting
D = D.to(device)
G = G.to(device)

# Binary Cross Entropy Loss and Optimizer
criterion = nn.BCELoss()
d_optimizer = torch.optim.Adam(D.parameters(), lr = learning_rate)
g_optimizer = torch.optim.Adam(G.parameters(), lr = learning_rate)

def denorm(x):
  output = (x+1)/2
  return output.clamp(0,1)

def reset_gradient():
  d_optimizer.zero_grad()
  g_optimizer.zero_grad()

# Start training
total_step = len(data_loader)
for epoch in  range(epochs) :
  for i, (images, _) in enumerate(data_loader) :
    # Move the flatten image to GPU
    images = images.reshape(batch_size, -1).to(device)

    # Create the Label which are later used as input for the BCE Loss
    real_labels = torch.ones(batch_size, 1).to(device) # 1
    fake_labels = torch.zeros(batch_size, 1).to(device) # 0


    # 1. Discriminator Training 
    outputs = D(images)
    d_loss_real = criterion(outputs, real_labels)
    real_score = outputs # ??

    # Compute BCELoss using fake images
    # 100 개의 Random vector (64차원) 생성
    # 100 개의 랜덤 이미지 생성
    z = torch.randn(batch_size, latent_size).to(device)
    fake_images = G(z)
    outputs = D(fake_images)
    d_loss_fake = criterion(outputs, fake_labels)
    fake_score = outputs

    # Backpropagation and Optimizing
    d_loss = d_loss_real + d_loss_fake
    reset_gradient()
    d_loss.backward() # gradient 계산
    d_optimizer.step() # gradient 기반으로 weight update


    # 2. Generator Training
    z = torch.randn(batch_size, latent_size).to(device)
    fake_images = G(z)
    outputs = D(fake_images)

    # minimizing log(1 - D(G(z)))
    # instead of it, we do maximize log(D(G(z)))
    # instead of it, we do minimize -log(D(G(z)))
    g_loss = criterion(outputs, real_labels)

    # Backpropagation and optimize
    reset_gradient()
    g_loss.backward()
    g_optimizer.step()

    if (i+1) % 100 == 0:
      print('Epoch [{}/{}], Step [{}/{}], d_loss: {:.4f}, g_loss: {:.4f}, D(x): {:.2f}, D(G(z)): {:.2f}'.format(
          epoch, epochs, i+1, total_step, d_loss.item(), g_loss.item(), real_score.mean().item(), fake_score.mean().item())
      )
  
  if epoch == 0 :
    # images.size(0) = batch.size 즉 100
    # channels = 1, image_size = 28*28
    # 원본 이미지
    images = images.reshape(images.size(0), 1, 28, 28)
    save_image(denorm(images), os.path.join(sample_dir, 'real_images.png'))
  
  # 생성한 image
  fake_images = fake_images.reshape(fake_images.size(0),1,28,28)
  save_image(denorm(fake_images), os.path.join(sample_dir, 'fake_images-{}.png'.format(epoch+1)))

# Save the model checkpoints
torch.save(G.state_dict(), 'G.dict')
torch.save(D.state_dict(), 'D,dict')
