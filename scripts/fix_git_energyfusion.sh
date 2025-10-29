#!/bin/bash
# 🔧 EnergyFusion Evolution Edition: automatic branch creation & publish

BRANCH="energyfusion_evolution"
REPO_NAME="HOT"

echo "🚀 Creating branch '$BRANCH' in repository $REPO_NAME..."

# Ensure we are in a git repo
if [ ! -d ".git" ]; then
  echo "❌ Not a Git repository. Please run inside repo root."
  exit 1
fi

# Create branch if not exists
git checkout -B $BRANCH

# Create directories
mkdir -p EnergyFusion/{agents,core,ui,models,configs}

# Add placeholder README if missing
if [ ! -f EnergyFusion/README.md ]; then
  echo "# ⚡ EnergyFusion Evolution Edition" > EnergyFusion/README.md
  echo "Autonomous energy market and weather intelligence engine." >> EnergyFusion/README.md
fi

# Add everything and commit
git add EnergyFusion scripts/fix_git_energyfusion.sh
git commit -m "Initialize EnergyFusion Evolution Edition structure" || echo "⚠️ No changes to commit."

# Push to origin
git push origin $BRANCH

echo "✅ EnergyFusion branch '$BRANCH' created and pushed successfully!"

