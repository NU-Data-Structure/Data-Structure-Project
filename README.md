# Git Workflow: Clone to Pull Request (Develop Branch)

This workflow shows the standard steps to contribute to a repo where `main` is the default branch.

---

## 1. Clone the repository
```
git clone https://github.com/NU-Data-Structure/Data-Structure-Project.git
cd Data-Structure-Project
```

## 2. Create a new feature branch
Never work directly on `main`. Use a descriptive branch name:
```
git checkout -b feature/your-feature-name
```

## 3. Make your changes
Edit code, add files, fix bugs, or implement features.

## 4. Stage and commit changes
```
git add .
git commit -m "Short, clear commit message describing the change"
```

## 5. Sync with the latest `develop`
Before pushing, make sure your branch includes the latest changes from `main`:
```
git fetch origin
git rebase origin/main
```


## 6. Push your feature branch to remote
```
git push origin feature/your-feature-name
```

## 7. Create a Pull Request (PR)
1. Go to the GitHub repo in your browser.  
2. Click **Compare & pull request** for your branch.  
3. Make sure the base branch is `main`.  
4. Assign reviewers, add a clear description, and submit the PR.

## 8. Address feedback
- Make additional commits on your branch if reviewers request changes.  
- Push them to the same branch; the PR updates automatically.

---

**Tip:** Always check which branch the team uses for integration (`main`) before rebasing or creating a PR. This prevents merge conflicts and ensures smooth collaboration.
