# riku.click DNS Configuration Review (Namecheap)

The screenshots provided from Namecheap show the initial DNS setup for **riku.click**. Below is a review of the current state and concrete adjustments required before the domain can safely point to the live Riku Hoitaa services.

## Current settings observed

- **Nameservers**: `Namecheap BasicDNS` (default registrar nameservers).
- **URL Redirect Record**: `@ → https://www.riku.click/` (unmasked redirect).
- **CNAME Record**: `www → parkingpage.namecheap.com` (30 min TTL).
- **TXT Record**: `@ → "v=spf1 include:spf.efwd.registrar-servers.com ~all"` (Namecheap email forwarding SPF).
- **PremiumDNS**: not enabled.

These defaults are expected on a fresh Namecheap domain, but they must be replaced with application-specific records once the backend and marketing site are ready.

## Required changes before go-live

1. **Replace the parking CNAME**
   - Update the `www` CNAME to point to the real hosting platform (e.g., Vercel, Cloud Run + Cloud CDN, Netlify).
   - Remove Namecheap's parking target to avoid exposing the placeholder landing page.

2. **Serve the apex domain directly**
   - Decide whether the root `riku.click` is hosted via the same platform as `www`.
   - Replace the URL redirect with `A` (and optional `AAAA`) records to your load balancer/CDN IPs once provisioned. Using only a redirect can cause loops when `www` also points back to the root.

3. **Reserve subdomains for application components**
   - Add CNAME or A records for `api.riku.click`, `app.riku.click`, `status.riku.click`, and `docs.riku.click` to align with the architecture plan.
   - Configure them to target the corresponding Cloud Run services or static hosting endpoints.

4. **Lock down TLS issuance**
   - Add CAA records (e.g., `0 issue "letsencrypt.org"`) to restrict certificate authorities once you know which CA will be used.

5. **Prepare email routing**
   - Replace the default SPF TXT record when you onboard a real mail provider (Google Workspace, Fastmail, etc.).
   - Add MX records, DKIM, and DMARC policies as part of the email setup so that support and billing addresses pass deliverability checks.

6. **Document automation**
   - Capture the final DNS zone in Infrastructure-as-Code (Terraform/Pulumi) together with TTLs to ensure the configuration can be recreated in staging/production.

## Next steps

1. Confirm which hosting provider will serve `www` and the apex domain, then update the A/CNAME records accordingly.
2. Provision TLS certificates (managed or Let’s Encrypt) after DNS changes propagate.
3. Implement the remaining planned subdomains and email records.
4. Track all changes in the domain launch checklist so stakeholders know when the domain is production-ready.

Once these updates are in place, the Namecheap zone will match the previously defined launch requirements and can be handed over to DevOps for automation.

## SSL/TLS follow-up based on registrar panel

The Namecheap SSL dashboard screenshot (PositiveSSL for `leoai.live`) confirms that the account already manages certificate issuance through the registrar. To avoid blockers when `riku.click` goes live:

1. **Issue the correct certificate** – Request or re-key a certificate specifically for `riku.click` (and `*.riku.click` if you need subdomains). The currently installed certificate applies to another domain and cannot be reused.
2. **Generate a CSR from the target platform** – If Cloud Run/Cloud CDN or another managed service terminates TLS, generate the CSR there; otherwise, create one via OpenSSL and upload it to Namecheap before downloading the signed certificate.
3. **Automate renewal reminders** – PositiveSSL certificates typically renew annually. Add the expiry date to the launch checklist and integrate monitoring/alerts so the new domain never lapses.
4. **Store private keys securely** – Ensure the private key that pairs with the CSR is stored in a secrets manager (Secret Manager, AWS Secrets Manager, Vault) with restricted access.

Document the issuance steps alongside the DNS automation so that future renewals follow the same hardened process.
