# Luo kansiot
mkdir -p EnergyFusion/{agents,core,ui,models,configs}

# Luo README
cat > EnergyFusion/README.md <<'EOF'
# ⚡ EnergyFusion Evolution Edition v0.3

EnergyFusion yhdistää sähkönhinta-, sää- ja kulutusmallit itseoppivaksi energiaverkkoagentiksi.

## Rakenteen pääosat
- **agents/**: Geneettiset ja oppivat agentit
- **core/**: Hinta- ja säätiedon yhdistäminen
- **ui/**: Streamlit / Dash käyttöliittymä
- **models/**: ML- ja neuroverkkomallit
- **configs/**: Ympäristö- ja asetusparametrit
