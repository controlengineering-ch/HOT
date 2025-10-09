import React, { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import { Alert, SafeAreaView, StyleSheet, Text, TouchableOpacity, View } from 'react-native';
import { Camera, PhotoFile, useCameraDevices } from 'react-native-vision-camera';
import Animated, { FadeInDown, FadeOutUp } from 'react-native-reanimated';
import { useIsFocused } from '@react-navigation/native';

import { processReceipt } from '../services/receiptService';
import { ReceiptDraft } from '../types/receipt';

const navy = '#0B1D3D';
const green = '#3CB371';
const yellow = '#FFD166';
const white = '#FFFFFF';

type ProcessingState = 'idle' | 'capturing' | 'processing';

export const ReceiptScannerScreen: React.FC = () => {
  const cameraRef = useRef<Camera | null>(null);
  const devices = useCameraDevices();
  const device = devices.back;
  const isFocused = useIsFocused();

  const [hasPermission, setHasPermission] = useState(false);
  const [status, setStatus] = useState<ProcessingState>('idle');
  const [preview, setPreview] = useState<PhotoFile | null>(null);
  const [draft, setDraft] = useState<ReceiptDraft | null>(null);
  const [rawText, setRawText] = useState<string>('');
  const [awaitingMoreChoice, setAwaitingMoreChoice] = useState(false);

  useEffect(() => {
    async function requestPermissions() {
      const cameraStatus = await Camera.requestCameraPermission();
      setHasPermission(cameraStatus === 'authorized');
    }

    requestPermissions();
  }, []);

  const handleCapture = useCallback(async () => {
    if (!cameraRef.current || status !== 'idle') {
      return;
    }

    try {
      setStatus('capturing');
      const photo = await cameraRef.current.takePhoto({
        flash: 'auto',
        qualityPrioritization: 'quality',
      });
      setPreview(photo);
      setStatus('processing');

      const result = await processReceipt(photo.path);
      setDraft(result.draft);
      setRawText(result.rawText);
      setAwaitingMoreChoice(true);
      setStatus('idle');
    } catch (error) {
      console.error('Failed to capture receipt', error);
      setStatus('idle');
      Alert.alert('Kuva epäonnistui', 'Tarkista kameran oikeudet ja yritä uudelleen.');
    }
  }, [status]);

  const resetScanner = useCallback(() => {
    setPreview(null);
    setDraft(null);
    setRawText('');
    setAwaitingMoreChoice(false);
  }, []);

  const instructions = useMemo(() => {
    if (status === 'processing') {
      return 'Prosessoidaan kuittia...';
    }

    if (awaitingMoreChoice) {
      return 'Lisää kuitteja? Valitse kyllä aloittaaksesi uuden skannauksen.';
    }

    if (!draft) {
      return 'Kohdista kuitti vihreään kehykseen – kamera laukaisee automaattisesti.';
    }

    return 'Kuitti tallennettu. Voit sulkea tai skannata uuden kuitin.';
  }, [awaitingMoreChoice, draft, status]);

  if (!device || !hasPermission) {
    return (
      <SafeAreaView style={[styles.container, styles.centered]}>
        <Text style={styles.errorText}>
          Kameralupa puuttuu. Myönnä lupa asetuksissa jatkaaksesi.
        </Text>
      </SafeAreaView>
    );
  }

  return (
    <SafeAreaView style={styles.container}>
      <View style={styles.cameraContainer}>
        {isFocused && status !== 'processing' && !preview ? (
          <Camera
            ref={cameraRef}
            style={StyleSheet.absoluteFill}
            device={device}
            isActive={true}
            photo={true}
            preset="high"
            focusable
          />
        ) : null}
        <View style={styles.overlay}>
          <View style={styles.frame} />
        </View>
        <View style={styles.instructionsContainer}>
          <Text style={styles.instructionsText}>{instructions}</Text>
        </View>
      </View>

      <View style={styles.bottomSheet}>
        {draft ? (
          <Animated.View entering={FadeInDown} exiting={FadeOutUp} style={styles.summaryContainer}>
            <Text style={styles.summaryTitle}>Litteroitu kuitti</Text>
            <Text style={styles.summaryText}>Summa: {draft.amount?.toFixed(2) ?? '-'} {draft.currency ?? ''}</Text>
            <Text style={styles.summaryText}>Veron osuus: {draft.taxAmount?.toFixed(2) ?? '-'}</Text>
            <Text style={styles.summaryText}>Päivämäärä: {draft.purchaseDate ?? '-'}</Text>
            <Text style={styles.summaryText}>Toimittaja: {draft.merchantName ?? '-'}</Text>
            <Text style={styles.summaryText}>Kustannuspaikka: {draft.costCenter ?? '-'}</Text>
            {draft.confirmationRequiredFields?.length ? (
              <Text style={styles.warningText}>
                Tarkista kentät: {draft.confirmationRequiredFields.join(', ')}
              </Text>
            ) : null}
            {awaitingMoreChoice ? (
              <View style={styles.morePromptContainer}>
                <Text style={styles.morePromptTitle}>Lisää kuitteja?</Text>
                <View style={styles.morePromptButtons}>
                  <TouchableOpacity
                    style={[styles.choiceButton, styles.choiceButtonPrimary]}
                    onPress={resetScanner}
                  >
                    <Text style={styles.choiceButtonPrimaryText}>Kyllä</Text>
                  </TouchableOpacity>
                  <TouchableOpacity
                    style={[styles.choiceButton, styles.choiceButtonSecondary]}
                    onPress={() => setAwaitingMoreChoice(false)}
                  >
                    <Text style={styles.choiceButtonSecondaryText}>Ei</Text>
                  </TouchableOpacity>
                </View>
              </View>
            ) : (
              <TouchableOpacity style={styles.secondaryButton} onPress={resetScanner}>
                <Text style={styles.secondaryButtonText}>Skannaa uusi kuitti</Text>
              </TouchableOpacity>
            )}
          </Animated.View>
        ) : null}

        <TouchableOpacity
          style={[
            styles.captureButton,
            (status !== 'idle' || awaitingMoreChoice) && styles.captureButtonDisabled,
          ]}
          onPress={handleCapture}
          disabled={status !== 'idle' || awaitingMoreChoice}
        >
          <Text style={styles.captureButtonText}>
            {status === 'processing' ? 'Prosessoidaan…' : 'Tallenna kuitti'}
          </Text>
        </TouchableOpacity>

        {rawText ? (
          <View style={styles.rawTextContainer}>
            <Text style={styles.rawTextTitle}>OCR raakateksti</Text>
            <Text style={styles.rawText}>{rawText}</Text>
          </View>
        ) : null}
      </View>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: navy,
  },
  centered: {
    alignItems: 'center',
    justifyContent: 'center',
    padding: 24,
  },
  cameraContainer: {
    flex: 2,
    backgroundColor: navy,
  },
  overlay: {
    ...StyleSheet.absoluteFillObject,
    alignItems: 'center',
    justifyContent: 'center',
  },
  frame: {
    width: '80%',
    height: '60%',
    borderColor: green,
    borderWidth: 3,
    borderRadius: 16,
    backgroundColor: 'rgba(11, 29, 61, 0.2)',
  },
  instructionsContainer: {
    position: 'absolute',
    bottom: 24,
    width: '100%',
    alignItems: 'center',
  },
  instructionsText: {
    color: white,
    fontSize: 16,
    textAlign: 'center',
    paddingHorizontal: 24,
  },
  bottomSheet: {
    flex: 1,
    backgroundColor: white,
    borderTopLeftRadius: 24,
    borderTopRightRadius: 24,
    paddingHorizontal: 24,
    paddingTop: 16,
  },
  captureButton: {
    backgroundColor: green,
    paddingVertical: 16,
    borderRadius: 16,
    alignItems: 'center',
    marginBottom: 16,
  },
  captureButtonDisabled: {
    backgroundColor: '#96d1b1',
  },
  captureButtonText: {
    color: navy,
    fontWeight: '600',
    fontSize: 18,
  },
  summaryContainer: {
    backgroundColor: '#F5F7FA',
    borderRadius: 16,
    padding: 16,
    marginBottom: 16,
  },
  morePromptContainer: {
    marginTop: 16,
  },
  morePromptTitle: {
    color: navy,
    fontSize: 16,
    fontWeight: '600',
    marginBottom: 12,
    textAlign: 'center',
  },
  morePromptButtons: {
    flexDirection: 'row',
    justifyContent: 'space-between',
  },
  choiceButton: {
    flex: 1,
    paddingVertical: 14,
    borderRadius: 14,
    alignItems: 'center',
    marginHorizontal: 6,
  },
  choiceButtonPrimary: {
    backgroundColor: green,
  },
  choiceButtonPrimaryText: {
    color: navy,
    fontSize: 18,
    fontWeight: '600',
  },
  choiceButtonSecondary: {
    borderWidth: 1,
    borderColor: '#B5C1D3',
    backgroundColor: white,
  },
  choiceButtonSecondaryText: {
    color: navy,
    fontSize: 18,
    fontWeight: '600',
  },
  summaryTitle: {
    color: navy,
    fontSize: 18,
    fontWeight: '700',
    marginBottom: 8,
  },
  summaryText: {
    color: navy,
    fontSize: 15,
    marginBottom: 4,
  },
  warningText: {
    color: yellow,
    fontSize: 14,
    marginTop: 8,
  },
  secondaryButton: {
    borderColor: navy,
    borderWidth: 1,
    borderRadius: 12,
    paddingVertical: 12,
    marginTop: 16,
    alignItems: 'center',
  },
  secondaryButtonText: {
    color: navy,
    fontWeight: '600',
  },
  rawTextContainer: {
    borderTopWidth: 1,
    borderTopColor: '#E2E6ED',
    paddingTop: 12,
  },
  rawTextTitle: {
    color: navy,
    fontSize: 16,
    fontWeight: '600',
    marginBottom: 4,
  },
  rawText: {
    color: '#4A5970',
    fontSize: 12,
    lineHeight: 16,
  },
  errorText: {
    color: yellow,
    textAlign: 'center',
    fontSize: 16,
  },
});

export default ReceiptScannerScreen;
