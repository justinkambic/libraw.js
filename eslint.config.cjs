/* Minimal flat ESLint config for this repo. This keeps things simple and
   avoids FlatCompat compatibility issues. It mirrors the small ruleset in
   .eslintrc.json: uses the TypeScript parser and enforces semicolons and
   single quotes. */
module.exports = [
  // skip distribution and prebuilt artifacts
  { ignores: ['dist/**', 'prebuilds/**', 'build/**'] },
  {
    languageOptions: {
      parser: require('@typescript-eslint/parser'),
      parserOptions: {
        ecmaVersion: 2020,
        sourceType: 'module',
      },
    },
    plugins: {
      '@typescript-eslint': require('@typescript-eslint/eslint-plugin'),
    },
    rules: {
      semi: ['error', 'always'],
      quotes: ['error', 'single'],
    },
  },
];
