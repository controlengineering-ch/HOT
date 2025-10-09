import type { Config } from 'tailwindcss';

const config: Config = {
  content: ['./index.html', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      colors: {
        primary: '#0B1D3D',
        accent: '#3CB371',
        warning: '#FFD166',
        surface: '#FFFFFF'
      }
    }
  },
  plugins: []
};

export default config;
